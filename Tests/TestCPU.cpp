//
// Created by Spencer Dixon on 5/6/20.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include <Emulator/CPU.h>

int main(int argc, char* argv[])
{

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // TODO(scd): do proper relative path utilities?
        printf("Current working dir: %s\n", cwd);
        char* path = "./../../Tests/loop.gb";

        CPU cpu(path);
        cpu.load();
    } else {
        perror("getcwd() error");
        return 1;
    }
    exit(1);

   return 0;
}