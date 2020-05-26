//
// Created by Spencer Dixon on 5/3/20.
//

#include "LogStream.h"
#include <inttypes.h>

LogStream::~LogStream() {
    char newline = '\n';
    write(&newline, 1);
}

LogStream dbg() {
    LogStream stream;
    return stream;
}

const LogStream &operator<<(const LogStream &stream, u8 value) {
    char buffer[8];
    sprintf(buffer, "%" PRIu8, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, u16 value) {
    char buffer[16];
    sprintf(buffer, "%" PRIu16, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, u32 value) {
    char buffer[32];
    sprintf(buffer, "%" PRIu32, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, u64 value) {
    char buffer[32];
    sprintf(buffer, "%" PRIu64, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, s8 value) {
    char buffer[8];
    sprintf(buffer, "%" PRId8, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, s16 value) {
    char buffer[16];
    sprintf(buffer, "%" PRId16, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, s32 value) {
    char buffer[32];
    sprintf(buffer, "%" PRId32, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, s64 value) {
    char buffer[32];
    sprintf(buffer, "%" PRId64, value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, void *value) {
    char buffer[32];
    sprintf(buffer, "%p", value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, f32 value) {
    char buffer[32];
    sprintf(buffer, "%.5f", value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, f64 value) {
    char buffer[32];
    sprintf(buffer, "%.5f", value);
    return stream << buffer;
}

const LogStream &operator<<(const LogStream &stream, bool value) {
    char buffer[32];
    sprintf(buffer, "%s", value ? "true" : "false");
    return stream << buffer;
}

const LogStream& operator<<(const LogStream& stream, const String& string) {
    stream.write(string.characters(), string.length());
    return stream;
}