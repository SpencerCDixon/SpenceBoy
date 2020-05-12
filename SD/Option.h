//
// Created by Spencer Dixon on 5/11/20.
//

#pragma once

template<typename T>
class Option {
public:
    Option() {}
    Option(const T& t)
        : m_none(false)
    {
        // Placement new -> you're telling the compiler where to store the memory
        new (&m_storage) T(t);
    }

    ~Option()
    {
        if (!m_none)
            (*reinterpret_cast<T*>(&m_storage)).~T();
    }

    bool is_none() const { return m_none; }

    const T& value() const {
        ASSERT(!m_none);
        return *reinterpret_cast<T*>(&m_storage);
    }

private:
    u8 m_storage[sizeof(T)];
    bool m_none { true };
};