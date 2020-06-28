//
// Created by Spencer Dixon on 6/28/20.
//

#include <SD/Assertions.h>
#include <SD/Test.h>
#include <SD/Vector.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    {
        Vector<u8> v;

        u8 second_value = 60;
        v.append(42);
        v.append(second_value);
        v.append(3);

        ASSERT(v[0] == 42);
        ASSERT(v[1] == 60);
        ASSERT(v.at(2) == 3);
        PASS("append()");
    }

    // Empty
    {
        Vector<bool> v;
        TEST_ASSERT(v.is_empty(), "is empty")
    }

    // First/Last
    {
        Vector<int> v;
        v.append(1);
        v.append(2);
        v.append(3);
        TEST_ASSERT(v.first() == 1, "first()")
        TEST_ASSERT(v.last() == 3, "last()")
    }

    // Contains
    {
        Vector<int> v { 1, 2, 42, 4, 5 };
        TEST_ASSERT(v.contains_slow(42), "contains_slow()")
    }

    // Copy Constructor
    {
        Vector<int> v1 { 1, 2, 3 };
        auto v2 = v1;
        ASSERT(v1.first() == 1);
        ASSERT(v2.first() == 1);
        PASS("copy constructor");
    }

    // Move Constructor
    {
        Vector<int> v1 { 1, 2, 3 };
        auto v2 = move(v1);
        ASSERT(v1.is_empty());
        ASSERT(v2.first() == 1);
        PASS("move constructor");
    }

    // Iteration
    {
        int sum = 0;
        Vector<int> v1 { 1, 2, 3 };

        for (auto& val : v1)
            sum += val;

        TEST_ASSERT(sum == 6, "iteration");
    }
}