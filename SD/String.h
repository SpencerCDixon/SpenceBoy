//
// Created by Spencer Dixon on 5/24/20.
//

#pragma once

#include <stdlib.h>

// No ref counting
// No byte buffer
// += primitives

class String {
public:
    static String* create_uninitialized(size_t length, char*& buffer);
    static String* create(const char* cstring, size_t length);
    static String* create(const char* cstring);
    static String* the_empty_string();

    size_t length() const { return m_length; }
    const char* characters() const { return &m_inline_buffer[0]; }

    // Andreas: How to properly clean up here?
    ~String();

private:
    enum class ConstructEmptyStringTag {
        ConstructEmptyString
    };
    explicit String(ConstructEmptyStringTag)
    {
        m_inline_buffer[0] = '\0';
    }

    enum class ConstructWithInlineBufferTag {
        ConstructWithInlineBuffer
    };
    String(ConstructWithInlineBufferTag, size_t length);

private:
    size_t m_length { 0 };
    char m_inline_buffer[0];
};
