//
// Created by Spencer Dixon on 6/28/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>
#include <SD/Vector.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    if (false)
        ASSERT_NOT_REACHED();

    dbg() << "Testing vector...";

    Vector<u8> bytes;

    u8 second_value = 60;
    bytes.append(42);
    bytes.append(second_value);

    ASSERT(bytes[0] == 42);
    ASSERT(bytes[1] == 60);

    // Empty
    {
        Vector<bool> v;
        ASSERT(v.is_empty());
    }

    // First/Last
    {
        Vector<int> v;
        v.append(1);
        v.append(2);
        v.append(3);
        ASSERT(v.first() == 1);
        ASSERT(v.last() == 3);
    }

    // Contains
    {
        Vector<int> v { 1, 2, 42, 4, 5 };
        ASSERT(v.contains_slow(42));
    }

    // Copy Constructor
    {
        Vector<int> v1 { 1, 2, 3 };
        auto v2 = v1;
        ASSERT(v1.first() == 1);
        ASSERT(v2.first() == 1);
    }

    // Move Constructor
    {
        Vector<int> v1 { 1, 2, 3 };
        auto v2 = move(v1);
        ASSERT(v1.is_empty());
        ASSERT(v2.first() == 1);
    }

    // Iteration
    {
        int sum = 0;
        Vector<int> v1 { 1, 2, 3 };

        for (auto& val : v1)
            sum += val;

        ASSERT(sum == 6);
    }
}