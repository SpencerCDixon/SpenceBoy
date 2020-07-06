//
// Created by Spencer Dixon on 5/11/20.
//

#pragma once

#include <SD/Assertions.h>
#include <SD/Types.h>

#include "new"

template<typename T>
class Option {
public:
    Option() { }
    Option(const T& value)
        : m_none(false)
    {
        // Placement new -> you're telling the compiler where to store the memory
        new (&m_storage) T(value);
    }

    ~Option()
    {
        if (!m_none)
            (*reinterpret_cast<T*>(&m_storage)).~T();
    }

    bool is_none() const { return m_none; }

    const T& value() const
    {
        ASSERT(!m_none);
        return *reinterpret_cast<const T*>(&m_storage);
    }

private:
    u8 m_storage[sizeof(T)] { 0 };
    bool m_none { true };
};