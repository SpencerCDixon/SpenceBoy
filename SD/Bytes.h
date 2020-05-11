//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

// clang-format off
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
// clang-format on

inline u16 to_le_16_bit(u8 byte1, u8 byte2)
{
    u16 result = byte2;
    result = (result << 8) | byte1;
    return result;
}

// NOTE: half carry is not used much in real world, was required for old hardware but new hardware
inline bool will_half_carry(u8 byte1, u8 byte2)
{
    byte2 = byte2 & 0b00001111;
    byte1 = byte1 & 0b00001111;
    u8 result = byte1 + byte2;
    u8 half_carry_flag = 1 << 4;
    return (result & (half_carry_flag)) == half_carry_flag;
}

inline bool will_carry(u8 byte1, u8 byte2)
{
    u16 result = byte1 + byte2;
    u16 carry_flag = 1 << 8;
    return (result & carry_flag) == carry_flag;
}

// Taken from: https://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
// Some adjustments were added by me to bring color
inline void hexDump(const char* desc, const void* addr, const int len)
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

            printf("  %04x ", i);
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