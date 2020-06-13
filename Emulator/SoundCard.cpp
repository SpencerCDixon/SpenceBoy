//
// Created by Spencer Dixon on 6/13/20.
//

#include "SoundCard.h"

u8 SoundCard::in(u16 address)
{
    dbg() << "SoundCard::in() " << to_hex(address);
    return 0;
}

void SoundCard::out(u16 address, u8 value)
{
    dbg() << "PPU::out(" << to_hex(address) << ", " << to_hex(value) << ")";
}
