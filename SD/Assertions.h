#pragma once

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define ASSERT assert
#define ASSERT_NOT_REACHED() assert(false)

inline void error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

inline void perror_exit_if(bool is_true, const char* fmt) {
    if (is_true) {
        perror(fmt);
        exit(1);
    }
}