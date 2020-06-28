//
// Created by Spencer Dixon on 6/28/20.
//

#pragma once

#include <SD/Assertions.h>
#include <SD/Types.h>
#include <SD/Utility.h>

#include <new>

template<typename T>
class Vector {
public:
    Vector() { }
    ~Vector()
    {
        clear();
    }

    T& at(size_t index)
    {
        ASSERT(index < m_size);
        return data()[index];
    }

    size_t capacity() const { return m_capacity; }
    size_t size() { return m_size; }
    bool is_empty() const { return size() == 0; }

    //
    // Insertion/Removal
    //

    template<class... Args>
    void push(Args&&... args)
    {
        grow_capacity(m_size + 1);
        new (slot(m_size)) T(forward<Args>(args)...);
        ++m_size;
    }

    void grow_capacity(size_t needed_capacity)
    {
        if (m_capacity >= needed_capacity)
            return;

        ensure_capacity(needed_capacity);
    }

    void ensure_capacity(size_t needed_capacity)
    {
        if (m_capacity >= needed_capacity)
            return;

        size_t new_capacity = needed_capacity;
        auto* new_data = (T*)malloc(new_capacity * sizeof(T));

        // Move items to new location
        for (size_t i = 0; i < m_size; ++i) {
            new (&new_data[i]) T(move(at(i)));
            at(i).~T();
        }

        if (m_data)
            free(m_data);

        m_data = new_data;
        m_capacity = new_capacity;
    }

private:
    void clear()
    {
        if (m_data) {
            free(m_data);
            m_data = nullptr;
        }
    }

    T* data() { return m_data; }

    T* slot(size_t i) { return &data()[i]; }
    const T* slot(size_t i) const { return &data()[i]; }

private:
    size_t m_capacity { 0 };
    size_t m_size { 0 };
    T* m_data { nullptr };
};