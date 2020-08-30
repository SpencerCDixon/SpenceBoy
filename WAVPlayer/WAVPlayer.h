//
// Created by spence on 8/29/20.
//

#pragma once

#include "RuntimeSettings.h"
#include "WAVFile.h"

class WAVPlayer {
public:
    WAVPlayer(RuntimeSettings settings);
    void play(const WAVFile& file);
    void play_simple(const char* file);
    void play_complex(const char* file);

//    void play_sine_wave();

    void print_devices();
private:
    RuntimeSettings m_settings;
};
