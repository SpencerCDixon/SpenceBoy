//
// Created by Spencer Dixon on 5/9/20.
//

#pragma once

inline u16 to_le_16_bit(u8 byte1, u8 byte2) {
    u16 result = byte2;
    result = (result << 8) | byte1;
    return result;
}

inline bool will_half_carry_sub(u8 original, u8 operand) {
    operand = operand & 0b00001111;
    original = original & 0b00001111;
    if (original == 0 && operand > 0) return true;

    return operand > original;
}