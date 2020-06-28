//
// Created by Spencer Dixon on 5/30/20.
//

#pragma once

template<typename T>
inline T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}

template<typename T>
struct RemoveReference {
    typedef T Type;
};
template<class T>
struct RemoveReference<T&> {
    typedef T Type;
};
template<class T>
struct RemoveReference<T&&> {
    typedef T Type;
};

template<class T>
inline constexpr T&& forward(typename RemoveReference<T>::Type& param)
{
    return static_cast<T&&>(param);
}


#define SD_MAKE_NONCOPYABLE(c) \
private:                       \
    c(const c&) = delete;      \
    c& operator=(const c&) = delete;

#define SD_MAKE_NONMOVABLE(c) \
private:                       \
    c(c&&) = delete;      \
    c& operator=(c&&) = delete;