//
// Created by Spencer Dixon on 6/4/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/String.h>

#include <time.h>
#include <unistd.h>

class Timer {
public:
    Timer(String name)
        : m_name(name)
        , m_start_time(clock())
    {
    }

    ~Timer()
    {
        clock_t total_time;
        total_time = clock() - m_start_time;
        double time_taken = ((double)total_time) / (CLOCKS_PER_SEC / 1000); // in ms
        dbg() << m_name << " took: " << time_taken << "ms";
    }

    void wait_until_elapsed_ms(int ms)
    {
        clock_t end_time;
        end_time = clock() - (clock() - m_start_time) + ms * CLOCKS_PER_SEC / 1000;
        while (clock() < end_time) { }
    }

private:
    String m_name;
    clock_t m_start_time;
};
