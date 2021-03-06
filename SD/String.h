//
// Created by Spencer Dixon on 5/24/20.
//

#pragma once

#include <SD/Assertions.h>
#include <SD/Vector.h>
#include <SD/Option.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class String {
public:
    String()
    {
        set_string("");
    }

    String(const char* cstring)
    {
        set_string(cstring);
    }

    String(const String& other)
    {
        set_string(other.m_characters);
    }

    String(String&& other)
    {
        m_characters = other.m_characters;
        m_length = other.m_length;

        other.m_length = 0;
        other.m_characters = nullptr;
    }
    String(const char* cstring, size_t length)
    {
        m_length = length;
        delete[] m_characters;
        m_characters = new char[m_length + 1];
        memcpy(m_characters, cstring, length);
        m_characters[length] = '\0';
    }

    // PERF: Needs to be optimized
    String& operator=(const String& other)
    {
        set_string(other.m_characters);
        return *this;
    }

    // PERF: Needs to be optimized
    ~String()
    {
        delete[] m_characters;
    }

    const char* characters() const { return m_characters; }
    char* characters() { return m_characters; }
    size_t length() const { return m_length; }

    String& operator+=(const String& other)
    {
        char* new_characters = new char[m_length + other.m_length + 1];
        new_characters[0] = '\0';
        strcat(new_characters, m_characters);
        strcat(new_characters, other.m_characters);
        delete[] m_characters;
        m_characters = new_characters;
        return *this;
    }

    enum class TrimLocation {
        Left,
        Right,
        Both
    };
    String trim_whitespace(TrimLocation location = TrimLocation::Both);
    String substring(size_t start, size_t length);
    Vector<String> split(char separator);

    Option<int> to_int();

private:
    void set_string(const char* cstring)
    {
        delete[] m_characters;
        m_length = strlen(cstring);
        m_characters = new char[m_length + 1];
        strcpy(m_characters, cstring);
    }

private:
    char* m_characters { nullptr };
    size_t m_length { 0 };
};

// PERF: These two operator+ calls are VERY slow and need to be optimized.
String operator+(const String&, const String&);
String operator+(const String&, const char*);

bool operator==(const String&, const String&);
bool operator!=(const String&, const String&);
