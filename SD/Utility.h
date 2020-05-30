//
// Created by Spencer Dixon on 5/30/20.
//

#pragma once

template<typename T>
inline T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}
