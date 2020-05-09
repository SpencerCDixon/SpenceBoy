//
// Created by Spencer Dixon on 5/6/20.
//

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <Emulator/CPU.h>

void test_loop()
{
    const char* path = "./../../Tests/data/loop.gb";

    CPU cpu;
    cpu.load_rom(path);
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
}

void test_ram()
{
    const char* path = "./../../Tests/data/ram.gb";

    CPU cpu;
    cpu.load_rom(path);
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();

}

int main(int argc, char* argv[])
{
//    test_loop();
    test_ram();
    return 0;
}