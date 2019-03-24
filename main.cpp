#include <iostream>
#include <thread>
#include <chrono>
#include <array>
#include <algorithm>
#include <functional>

#include "ppm.hpp"
#include "util.hpp"

vector<uint32_t> bounds(uint32_t parts, size_t mem)
{
    std::vector<uint32_t> ret(parts+1);
    uint32_t delta = mem / parts;
    for(int i=0; i<parts; ++i)
        ret[i] = delta * i;
    ret.back() = mem;
    return ret;
}

template<typename _Pt>
void tst(ppm<_Pt>& in, ppm<_Pt>& out, uint32_t _l, uint32_t _r)
{
    constexpr float filter[] = {
        0.10179640718562874, 0.11377245508982035, 0.10179640718562874,
        0.11377245508982035, 0.1377245508982036, 0.11377245508982035,
        0.10179640718562874, 0.11377245508982035, 0.10179640718562874
    };

    for(int i = _l; i < _r; ++i)
    {
        int y = i / in.cols();
        int x = i - y * in.cols();

        // Begin processing;
        static auto index_legal = [&](int x, int y){
            return x >= 0 && x < in.cols() && y >= 0 && y < in.rows();
        };

        _Pt new_pixels[3] = {0, 0, 0};
        const std::array<std::pair<int, int>, 9> positions = {
            std::make_pair(x-1, y-1),
            std::make_pair(x, y-1),
            std::make_pair(x+1, y-1),
            std::make_pair(x-1, y),
            std::make_pair(x, y),
            std::make_pair(x+1, y),
            std::make_pair(x-1, y-1),
            std::make_pair(x, y-1),
            std::make_pair(x+1, y+1)
        };

        for(int j = 0; j < 9; ++j)
            if(index_legal(positions[j].first, positions[j].second))
            {
                new_pixels[0] += filter[j] * in.r[ positions[j].second*in.cols() + positions[j].first ];
                new_pixels[1] += filter[j] * in.g[ positions[j].second*in.cols() + positions[j].first ];
                new_pixels[2] += filter[j] * in.b[ positions[j].second*in.cols() + positions[j].first ];
            }

        out.r[i] = new_pixels[0];
        out.g[i] = new_pixels[1];
        out.b[i] = new_pixels[2];
    }
}

int main()
{   
    std::string input_file = "test2.ppm";
    ppm<uint8_t> img(input_file); 

    ppm<uint8_t> out(img.rows(), img.cols());

    // Multi-threading part - begin
    Timer t;

    constexpr uint32_t parts = 11;

    auto bnd = bounds(parts, img.size());

    std::array<std::thread, parts - 1> thread_pool;

    for(int i = 0; i < parts - 1; ++i) // Use multi-threading. "-1" for the main thread.
        thread_pool[i] = std::thread(
            tst<decltype(img)::PixelType>, std::ref(img), std::ref(out), bnd[i], bnd[i+1]
            );

    // the main thread can sth. too.
    tst<decltype(img)::PixelType>(std::ref(img), std::ref(out), *(bnd.end()-2), bnd.back());

    // Join them.
    std::for_each(thread_pool.begin(), thread_pool.end(), std::mem_fn(&std::thread::join));
    // Multi-threading part - end

    std::cout << "Multi-threading: " << t.time_out() << " ms\n";

    t.reset();
    // For non-multi-threading:
    tst<decltype(img)::PixelType>(std::ref(img), std::ref(out), 0, bnd.back());
    std::cout << "Non-multi-threading: " << t.time_out() << " ms\n";

    input_file.insert(input_file.find_first_of('.'), "-postprocess");
    out.imwrite(input_file);
}