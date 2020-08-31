//
// Created by Spencer Dixon on 6/13/20.
//

#pragma once

#include "IODevice.h"

class SoundCard final : public IODevice {
public:
    u8 in(u16 address) override;
    void out(u16 address, u8 value) override;
};
