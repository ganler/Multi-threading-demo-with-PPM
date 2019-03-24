#pragma once

#include<iostream>
#include<sstream>
#include<exception>
#include<fstream>
#include<vector>
#include<cstdint>
#include<string>
#include<numeric>

template<typename _PixelType = uint8_t>
class ppm
{
public:
    using PixelType = _PixelType;
    std::vector<_PixelType> r;
    std::vector<_PixelType> g;
    std::vector<_PixelType> b;

    ppm() {};
    ppm(const std::string&);
    ppm(const ppm&) = delete; // TODO.
    ppm(const uint32_t, const uint32_t, int = 0);

    void imread(const std::string&);
    void imwrite(const std::string&);

    uint32_t cols();
    uint32_t rows();
    size_t size();

    const _PixelType max_pixel_val = std::numeric_limits<_PixelType>::max();

private:
    uint32_t m_rows = 0;
    uint32_t m_cols = 0; 
    size_t m_size = 0;
}; // class ppm

template<typename _PixelType>
ppm<_PixelType>::ppm(const std::string& _fname){
    imread(_fname);
}

template<typename _PixelType>
uint32_t ppm<_PixelType>::cols()
{
    return m_cols;
}

template<typename _PixelType>
uint32_t ppm<_PixelType>::rows()
{
    return m_rows;
}

template<typename _PixelType>
size_t ppm<_PixelType>::size()
{
    return m_size;
}

template<typename _PixelType>
ppm<_PixelType>::ppm(const uint32_t _rn, const uint32_t _cn, int _fill)
{
    m_rows = _rn;
    m_cols = _cn;
    m_size = m_rows * m_cols;

    r.resize(m_size);
    g.resize(m_size);
    b.resize(m_size);

    for(uint32_t i = 0; i < m_size; ++i)
        r[i] = g[i] = b[i] = _fill;

}

template<typename _PixelType>
void ppm<_PixelType>::imread(const std::string& _fname)
{
    std::ifstream src(_fname.c_str(), std::ios::in | std::ios::binary);
    
    if(src.is_open()) // init
    {
        std::string line;
        std::getline(src, line);

        if(line != "P6")
        {
            std::cout << "Error format!\n";
            return;
        }

        std::getline(src, line);
        while(line[0] == '#') // Skip the comments.
            std::getline(src, line);
        
        std::stringstream dim(line);

        try{
            dim >> m_cols;
            dim >> m_rows;
        } catch (std::exception &e){
            std::cout << "Header file format error: " << e.what() << '\n';
            return;
        }
        
        std::getline(src, line);
        std::stringstream max_val(line);

        {
            uint32_t actual_max_pixel_val;
            max_val >> actual_max_pixel_val;
            if(actual_max_pixel_val != max_pixel_val)
            {
                std::cout << "Header file error, your file\'s max pixel value should be " << max_pixel_val << ", but actually " << actual_max_pixel_val << '\n';
                return;
            }
        }
        
        m_size = m_cols * m_rows;

        r.resize(m_size);
        g.resize(m_size);
        b.resize(m_size);
        // pre-processin done;

        char aux;
        for(uint32_t i = 0; i < m_size; ++i)
        {
            src.read(&aux, 1);
            r[i] = aux;
            src.read(&aux, 1);
            g[i] = aux;
            src.read(&aux, 1);
            b[i] = aux;
        }
    }
    else
        std::cout << "Error, file cannot be opened\n";
    src.close();
}

template<typename _PixelType>
void ppm<_PixelType>::imwrite(const std::string& _fname)
{
    std::ofstream out(_fname.c_str(), std::ios::out | std::ios::binary);

    if(out.is_open())
    {
        out << "P6\n";
        out << m_cols;
        out << " ";
        out << m_rows;
        out << "\n" << static_cast<uint32_t>(max_pixel_val) << "\n";

        char aux;
        for(uint32_t i = 0; i < m_size; ++i)
        {
            out.write(&(aux = r[i]), 1);
            out.write(&(aux = g[i]), 1);
            out.write(&(aux = b[i]), 1);
        }
    }
    else
        std::cout << "Error, file cannot be opened\n";
    out.close();
}