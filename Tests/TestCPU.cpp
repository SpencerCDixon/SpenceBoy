//
// Created by Spencer Dixon on 5/6/20.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include <Emulator/CPU.h>

int main(int argc, char *argv[]) {

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // TODO(scd): do proper relative path utilities?
        printf("Current working dir: %s\n", cwd);
        const char *path = "./../../Tests/loop.gb";

        CPU cpu;
        cpu.load_rom(path);
        cpu.write(0xc002, 42);
        u8 data;
        data = cpu.read(0xc002);

        cpu.step();
        cpu.step();
        cpu.step();
        cpu.step();
        cpu.step();
        cpu.step();
        cpu.step();
        cpu.step();
        cpu.step();
        // TODO: Step until program is over
    } else {
        perror("getcwd() error");
        return 1;
    }
    return 0;
}