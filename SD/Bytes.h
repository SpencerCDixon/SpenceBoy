//
// Created by Spencer Dixon on 5/9/20.
//

#include <SD/String.h>
#include <SD/Types.h>

#pragma once

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define WHITE "\033[97m"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

inline String to_bits(u8 value)
{
    char buf[11] = { '0', 'b' };
    snprintf(buf + 2, 9, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(value));
    return String(buf);
}

inline String to_hex(u8 value)
{
    char buf[5] = { '0', 'x' };
    snprintf(buf + 2, 3, "%x", value);
    return String(buf);
}

inline String to_hex(u16 value)
{
    char buf[7] = { '0', 'x' };
    snprintf(buf + 2, 5, "%x", value);
    return String(buf);
}

inline u16 to_le_16_bit(u8 byte1, u8 byte2)
{
    return ((byte2 << 8) | byte1);
}

// NOTE: half carry is not used much in real world, was required for old hardware but new hardware
// doesn't often take advantage of it
inline bool will_half_carry(u8 byte1, u8 byte2)
{
    byte2 = byte2 & 0b00001111;
    byte1 = byte1 & 0b00001111;
    u8 result = byte1 + byte2;
    u8 half_carry_flag = 1 << 4;
    return (result & (half_carry_flag)) == half_carry_flag;
}

inline u8 swap_nibbles(u8 byte)
{
    return ((byte & 0x0f) << 4 | (byte & 0xf0) >> 4);
}

inline bool will_carry(u8 byte1, u8 byte2)
{
    u16 result = byte1 + byte2;
    u16 carry_flag = 1 << 8;
    return (result & carry_flag) == carry_flag;
}

inline bool will_carry(u16 original, u8 value_to_add)
{
    u32 result = original + value_to_add;
    u32 carry_flag = 1 << 16;
    return (result & carry_flag) == carry_flag;
}

inline bool will_carry(u16 original, u16 value_to_add)
{
    u32 result = original + value_to_add;
    u32 carry_flag = 1 << 16;
    return (result & carry_flag) == carry_flag;
}

inline bool will_carry_from_left_shift(u8 byte1)
{
    u16 result = byte1 << 1;
    u16 carry_flag = 1 << 8;
    return (result & carry_flag) == carry_flag;
}

inline u64 checksum(const unsigned char* buff, size_t len)
{
    u64 sum;
    for (sum = 0; len != 0; len--)
        sum += *(buff++);
    return sum;
}

// Taken from: https://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
// Some adjustments were added by me to bring color
inline void hex_dump(const char* desc, const void* addr, const int len, const int offset)
{
    int i;
    unsigned char buff[17];
    const unsigned char* pc = (const unsigned char*)addr;

    // Output description if given.

    if (desc != NULL)
        printf("%s:\n", desc);

    // Length checks.

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    } else if (len < 0) {
        printf("  NEGATIVE LENGTH: %d\n", len);
        return;
    }

    // Process every byte in the data.

    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Don't print ASCII buffer for the "zeroth" line.

            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.

            printf("  %04x ", i + offset);
        }

        // Now the hex code for the specific character.
        if (pc[i] > 0) {
            printf(GREEN " %02x" RESET, pc[i]);
        } else {
            printf(" %02x", pc[i]);
        }

        // And buffer a printable ASCII character for later.

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.

    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII buffer.

    printf("  %s\n", buff);
}