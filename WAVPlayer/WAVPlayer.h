//
// Created by spence on 8/29/20.
//

#pragma once

#include "RuntimeSettings.h"

class WAVPlayer {
public:
    WAVPlayer(RuntimeSettings settings);
    void play();
private:
    RuntimeSettings m_settings;
};
