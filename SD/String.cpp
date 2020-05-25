//
// Created by Spencer Dixon on 5/24/20.
//
#include <SD/Assertions.h>

#include "String.h"

#include <new>
#include <string.h>

static String* s_the_empty_string = nullptr;
String* String::the_empty_string()
{
    if (!s_the_empty_string) {
        void* slot = malloc(sizeof(String) + sizeof(char));
        s_the_empty_string = new (slot) String(ConstructEmptyStringTag::ConstructEmptyString);
    }
    return s_the_empty_string;

}

static inline size_t allocation_size_for_string(size_t length)
{
    //     members              string length             null byte char
    return sizeof(String) + (sizeof(char) * length) + sizeof(char);
}

String::String(ConstructWithInlineBufferTag, size_t length)
    : m_length(length)
{
}

String* String::create_uninitialized(size_t length, char*& buffer)
{
    ASSERT(length);
    // Andreas: So this puts the string into our slot but how do I properly clear the slot if I don't have the address?
    // Will a free(this) do it in ~String()??
    void* slot = malloc(allocation_size_for_string(length));
    ASSERT(slot);
    auto new_string = new (slot) String(ConstructWithInlineBufferTag::ConstructWithInlineBuffer, length);
    buffer = const_cast<char*>(new_string->characters());
    buffer[length] = '\0';
    return new_string;
}

String::~String()
{
    free(this);
}

String* String::create(const char* cstring, size_t length)
{
    if (!cstring)
        return nullptr;

    if (!length || !*cstring)
        return the_empty_string();

    char* buffer;
    auto new_string = create_uninitialized(length, buffer);
    memcpy(buffer, cstring, length * sizeof(char));
    return new_string;
}

String* String::create(const char* cstring)
{
    if (!cstring)
        return nullptr;

    return create(cstring, strlen(cstring));
}

