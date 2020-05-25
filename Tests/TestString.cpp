//
// Created by Spencer Dixon on 5/24/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>
#include <SD/String.h>

#include <stdio.h>
#include <string.h>

void assert_str_eq(String& lhs, const char* rhs)
{
    ASSERT(strcmp(lhs.characters(), rhs) == 0);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    printf("Testing String class... \n");
    printf("--------------------------\n\n");

    printf("size of raw String %lu\n", sizeof(String));

    // Concat
    {
        String s1;
        s1 = "hello";
        s1 += " there!";
        assert_str_eq(s1, "hello there!");
        printf("%s\n", s1.characters());

        String s2 = s1 + " this is a test!";
        assert_str_eq(s2, "hello there! this is a test!");
        printf("%s\n", s2.characters());
    }

    // Log Streaming
    {
        auto s1 = String("logging to debug stream");
        dbg() << s1;
    }

    // Equality
    {
        ASSERT(String("equal") == String("equal"));
        ASSERT(String("equal") != String("notequal"));
    }
}