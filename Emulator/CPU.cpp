//
// Created by Spencer Dixon on 5/6/20.
//

#include "CPU.h"
#include <SD/LogStream.h>

#include <stdio.h>

#define MAX_ROM_SIZE 32768

void CPU::load() {
    dbg() << "CPU::load() parsing op codes from ROM path: " << m_rom_path;

    FILE *fp;
    fp = fopen(m_rom_path, "rb");
    if (fp == NULL) {
        perror("CPU::load() Error: ");
        return;
    }

    char buff[MAX_ROM_SIZE];
    fread(buff, 10, 1, fp);

    for (int i = 0; i < 10; ++i) {
        if (buff[i] == 0x3e) {
            dbg() << "LD OpCode";
        }
        printf("%x ", buff[i]);
//        dbg() << "index: " << i << "byte: " << buff[i];
    }
}
