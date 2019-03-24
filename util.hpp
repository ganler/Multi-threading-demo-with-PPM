#pragma once

// Header only
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

class Timer
{
public:
    Timer() : clk_beg(high_resolution_clock::now()) {}
    void reset(){clk_beg = high_resolution_clock::now(); }

    template <typename Format = milliseconds>
    uint64_t time_out() const
    {
        return duration_cast<Format>(high_resolution_clock::now() - clk_beg).count();
    }

    uint64_t time_out_microseconds() const{ return time_out<microseconds>(); }
    uint64_t time_out_seconds() const{ return time_out<seconds>(); }
    uint64_t time_out_minutes() const{ return time_out<minutes>(); }
    uint64_t time_out_hours() const{ return time_out<hours>(); }

private:
    time_point<high_resolution_clock> clk_beg;
};// class Timer
