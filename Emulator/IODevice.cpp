//
// Created by Spencer Dixon on 6/7/20.
//

#include "IODevice.h"

static DummyIODevice s_the;

DummyIODevice& DummyIODevice::the()
{
    return s_the;
}
