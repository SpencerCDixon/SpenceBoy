//
// Created by Spencer Dixon on 5/24/20.
//

#pragma once

#include <stdlib.h>
#include <stdio.h>
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

    String& operator=(const String& other)
    {
        set_string(other.m_characters);
        return *this;
    }

    ~String()
    {
        delete[] m_characters;
    }

    const char* characters() const { return m_characters; }
    char* characters() { return m_characters; }
    size_t length() { return m_length; }

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

String operator+(const String&, const String&);
String operator+(const String&, const char*);
bool operator==(const String&, const String&);
bool operator!=(const String&, const String&);
