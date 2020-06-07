//
// Created by Spencer Dixon on 6/7/20.
//

#include "IODevice.h"

static DummyIODevice* s_the;

DummyIODevice& DummyIODevice::the()
{
    if (!s_the)
        s_the = new DummyIODevice;

    return *s_the;
}
