//
// Created by Spencer Dixon on 5/24/20.
//

#include <SD/String.h>

#include <stdio.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    printf("Testing String class... \n");
    printf("--------------------------\n\n");

    printf("size of raw String %lu\n", sizeof(String));
    printf("size of the empty String %lu\n", sizeof(String::the_empty_string()));

    {
        auto string = String::create("hello", 5);
        printf("size of 'hello' String %lu\n", sizeof(string));

        for (int i = 0; i < 5; ++i) {
            printf("char: %c\n", string->characters()[i]);
        }
    }

    auto s2 = String::create("world");
    printf("size of 'world' String %lu\n", sizeof(s2));

    for (int i = 0; i < 5; ++i) {
        printf("char: %c\n", s2->characters()[i]);
    }

}