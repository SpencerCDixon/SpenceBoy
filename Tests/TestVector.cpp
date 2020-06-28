//
// Created by Spencer Dixon on 6/28/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>
#include <SD/Vector.h>

#include <stdio.h>
#include <string.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    if (false)
        ASSERT_NOT_REACHED();

    dbg() << "Testing vector...";

    Vector<u8> bytes;

    u8 second_value = 60;
    bytes.push(42);
    bytes.push(second_value);

    ASSERT(bytes.at(0) == 42);
    ASSERT(bytes.at(1) == 60);
}