//
// Created by Spencer Dixon on 5/24/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>
#include <SD/String.h>
#include <SD/Utility.h>
#include <SD/Test.h>

#include <stdio.h>
#include <string.h>

void assert_str_eq(String& lhs, const char* rhs)
{
    ASSERT(strcmp(lhs.characters(), rhs) == 0);
}

class Entity {
public:
    Entity(String&& name)
        : m_name(move(name))
    {
    }
private:
    String m_name;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Construction
    {
        String s1("long string", 4);
        assert_str_eq(s1, "long");
        PASS("partial cstring constructor");
    }

    // Substring
    {
        String s1("hello world");
        auto s2 = s1.substring(0, 5);
        assert_str_eq(s2, "hello");
        auto s3 = s1.substring(6, 5);
        assert_str_eq(s3, "world");
        PASS("substring");
    }

    // Split
    {
        String s1("hello world");
        auto r1 = s1.split(' ');
        assert_str_eq(r1[0], "hello");
        assert_str_eq(r1[1], "world");

        String s2("one,two,three,four,  five");
        auto r2 = s2.split(',');
        assert_str_eq(r2[0], "one");
        assert_str_eq(r2[1], "two");
        assert_str_eq(r2[2], "three");
        assert_str_eq(r2[3], "four");
        assert_str_eq(r2[4], "  five");
        PASS("split");
    }

    // Concat
    {
        String s1;
        s1 = "hello";
        s1 += " there!";
        assert_str_eq(s1, "hello there!");

        String s2 = s1 + " this is a test!";
        assert_str_eq(s2, "hello there! this is a test!");
        PASS("concat");
    }

    // Log Streaming
    {
        auto s1 = String("logging to debug stream");
        dbg() << s1;
        PASS("log streaming");
    }

    // Equality
    {
        ASSERT(String("equal") == String("equal"));
        ASSERT(String("equal") != String("notequal"));
        PASS("equality checks");
    }

    // Move Constructor
    {
        auto my_string = String("this will be moved elsewhere");
        auto original_length = my_string.length();
        auto other = move(my_string);
        ASSERT(my_string.length() == 0);
        ASSERT(other.length() == original_length);
        assert_str_eq(other, "this will be moved elsewhere");

        auto should_move = String("should be moved!");
        auto entity = Entity(move(should_move));
        PASS("move constructor");
    }

    // Trimming
    {
        auto right_space = String("with new line\n\t ");
        TEST_ASSERT(right_space.trim_whitespace(String::TrimLocation::Right) == "with new line", "trim_whitespace(Right)");

        auto left_space = String("     lots of space to left");
        TEST_ASSERT(left_space.trim_whitespace(String::TrimLocation::Left) == "lots of space to left", "trim_whitespace(Left)");

        auto both = String("  \twhitespace on both sides\n  ");
        TEST_ASSERT(both.trim_whitespace() == "whitespace on both sides", "trim_whitespace(Both)");
    }

    // to_int
    {
        String s1("42");
        TEST_ASSERT(s1.to_int().value() == 42, "to_int() with valid number");

        String s2("blah");
        TEST_ASSERT(s2.to_int().is_none(), "to_int() with invalid number is none");

        String s3("0");
        TEST_ASSERT(s3.to_int().value() == 0, "to_int() 0 works");
    }
}
