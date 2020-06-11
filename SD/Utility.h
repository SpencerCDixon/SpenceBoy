//
// Created by Spencer Dixon on 5/30/20.
//

#pragma once

template<typename T>
inline T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}

#define SD_MAKE_NONCOPYABLE(c) \
private:                       \
    c(const c&) = delete;      \
    c& operator=(const c&) = delete;

#define SD_MAKE_NONMOVABLE(c) \
private:                       \
    c(c&&) = delete;      \
    c& operator=(c&&) = delete;