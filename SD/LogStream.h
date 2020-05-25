//
// Created by Spencer Dixon on 5/3/20.
//


#pragma once

#include <SD/Types.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

class LogStream {
public:
    LogStream() {}

    ~LogStream();

    // TODO(scd): Create a 'Write' interface so I could pass in-mem write for tests
    void write(const char *characters, int length) const {
        fwrite(characters, length, 1, stdout);
    }
};

inline const LogStream &operator<<(const LogStream &stream, const char *value) {
    if (!value)
        return stream << "(nullptr)";
    size_t length = strnlen(value, MAX_BUFFER_SIZE);
    stream.write(value, length);
    return stream;
}

LogStream dbg();

// Unsigned
const LogStream& operator<<(const LogStream&, u8);
const LogStream& operator<<(const LogStream&, u16);
const LogStream& operator<<(const LogStream&, u32);
const LogStream& operator<<(const LogStream&, u64);

// Signed
const LogStream& operator<<(const LogStream&, s8);
const LogStream& operator<<(const LogStream&, s16);
const LogStream& operator<<(const LogStream&, s32);
const LogStream& operator<<(const LogStream&, s64);

// Floating
const LogStream& operator<<(const LogStream&, f32);
const LogStream& operator<<(const LogStream&, f64);

// Other
const LogStream& operator<<(const LogStream&, void*);
const LogStream& operator<<(const LogStream&, bool);