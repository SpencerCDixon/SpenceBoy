//
// Created by Spencer Dixon on 6/6/20.
//

#pragma once

#include <SD/LogStream.h>
#include <SD/Types.h>

class IODevice {
    virtual u8 read(u16 address) = 0;
    virtual void write(u16 address, u8 value) = 0;
};

class DummyIODevice final : public IODevice {
public:
    virtual u8 read(u16 address) override
    {
        dbg() << "DummyIODevice::read(address: " << address << ")";
        return 0;
    }
    virtual void write(u16 address, u8 value) override
    {
        dbg() << "DummyIODevice::write(address: " << address << ", value: " << value << ")";
    }
};