#include "String.h"

#include <string.h>

String operator+(const String& lhs, const String& rhs)
{
    String tmp = lhs;
    tmp += rhs;
    return tmp;
}

String operator+(const String& lhs, const char* rhs)
{
    String tmp = lhs;
    tmp += rhs;
    return tmp;
}

bool operator==(const String& lhs, const String& rhs)
{
    return strcmp(lhs.characters(), rhs.characters()) == 0;
}

bool operator!=(const String& lhs, const String& rhs)
{
    return strcmp(lhs.characters(), rhs.characters()) != 0;
}

String String::trim_whitespace_right()
{
    auto is_whitespace_character = [](char ch) -> bool {
        return ch == '\t'
            || ch == '\n'
            || ch == '\v'
            || ch == '\f'
            || ch == '\r'
            || ch == ' ';
    };

    size_t substring_length = length();

    for (size_t i = length() - 1; i > 0; --i) {
        if (substring_length == 0)
            return "";
        if (!is_whitespace_character(characters()[i]))
            break;
        --substring_length;
    }

    char* buffer = (char*)malloc(substring_length + 1);
    memcpy(buffer, characters(), substring_length);
    buffer[substring_length] = '\0';
    set_string(buffer);
    free(buffer);

    return *this;
}

String String::trim_whitespace_left()
{
    auto is_whitespace_character = [](char ch) -> bool {
        return ch == '\t'
               || ch == '\n'
               || ch == '\v'
               || ch == '\f'
               || ch == '\r'
               || ch == ' ';
    };

    size_t substring_start = 0;
    size_t substring_length = length();

    for (size_t i = 0; i < length(); ++i) {
        if (substring_length == 0)
            return "";
        if (!is_whitespace_character(characters()[i]))
            break;
        ++substring_start;
        --substring_length;
    }

    char* buffer = (char*)malloc(substring_length + 1);
    memcpy(buffer, characters() + substring_start, substring_length);
    buffer[substring_length] = '\0';
    set_string(buffer);
    free(buffer);

    return *this;
}
