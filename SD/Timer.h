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
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        m_start_nanoseconds = ts.tv_nsec;
    }

    ~Timer()
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        long time_taken = (now.tv_nsec - m_start_nanoseconds) / 1000000;

        // Andreas: Whaaat!?
        if (time_taken < 0)
            return;

        dbg() << m_name << " took: " << (u32)time_taken << "ms";
    }

    void wait_until_elapsed_ms(double ms)
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        struct timespec sleep_request;
        sleep_request.tv_sec = 0;
        sleep_request.tv_nsec = (ms * 1000000) - (now.tv_nsec - m_start_nanoseconds);

        nanosleep(&sleep_request, nullptr);
    }

private:
    String m_name;
    long m_start_nanoseconds;
};
