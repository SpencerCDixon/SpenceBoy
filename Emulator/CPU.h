//
// Created by Spencer Dixon on 5/6/20.
//

#pragma once

class CPU {

public:
    CPU(char *rom_path) : m_rom_path(rom_path) {}

    void load();

private:
    char *m_rom_path;
};