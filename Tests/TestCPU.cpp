//
// Created by Spencer Dixon on 5/6/20.
//

#include <Emulator/CPU.h>

void test_loop()
{
    const char* path = "./../Tests/data/loop.gb";

    CPU cpu;
    cpu.load_rom(path);
    while (cpu.step());
}

void test_ram()
{
    const char* path = "./../Tests/data/ram.gb";

    CPU cpu;
    cpu.load_rom(path);
    while (cpu.step());
}

void test_smiley()
{
    const char* path = "./../Tests/data/smiley.gb";

    CPU cpu;
    cpu.load_rom(path);
    while (cpu.step());

}

int main(int argc, char* argv[])
{
    test_smiley();
    return 0;
}