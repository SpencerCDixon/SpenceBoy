//
// Created by Spencer Dixon on 5/24/20.
//

#include <SD/String.h>
#include <SD/Assertions.h>

#include <stdio.h>
#include <string.h>

void assert_str_eq(String& lhs, const char* rhs) {
    ASSERT(strcmp(lhs.characters(), rhs) == 0);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    printf("Testing String class... \n");
    printf("--------------------------\n\n");

    printf("size of raw String %lu\n", sizeof(String));

    String s1;
    s1 = "hello";
    s1 += " there!";
    assert_str_eq(s1, "hello there!");
    printf("%s\n", s1.characters());

    String s2 = s1 + " this is a test!";
    printf("%s\n", s2.characters());
}