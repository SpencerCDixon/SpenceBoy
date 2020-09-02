//
// Created by Spencer Dixon on 6/4/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/String.h>

#include <time.h>
#include <unistd.h>

class DebugTimer {
public:
    DebugTimer(String name)
        : m_name(name)
    {
        m_start_time = now();
    }

    ~DebugTimer()
    {
        f64 time_taken = (now() - m_start_time) / 1000000;
        dbg() << m_name << " took: " << (u32)time_taken << "ms";
    }

    void wait_until_elapsed_ms(double ms)
    {
        struct timespec sleep_request;
        sleep_request.tv_sec = 0;
        sleep_request.tv_nsec = (ms * 1000000) - (now() - m_start_time);

        nanosleep(&sleep_request, nullptr);
    }

    f64 now()
    {
        struct timespec now;
        perror_exit_if(clock_gettime(CLOCK_MONOTONIC, &now) < 0, "clock_gettime()");

        // ts.tv_nsec rolls to 0 every second. We can get accurate measurement by multiplying
        // by the second
        f64 timestamp = now.tv_sec * 1000000000 + now.tv_nsec;
        return timestamp;
    }

private:
    String m_name;
    f64 m_start_time { 0 };
};
