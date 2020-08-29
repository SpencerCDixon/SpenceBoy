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
private:
    RuntimeSettings m_settings;
};
