//
// Created by spence on 8/29/20.
//

#include <SD/LogStream.h>

#include "WAVPlayer.h"

WAVPlayer::WAVPlayer(RuntimeSettings settings)
    : m_settings(settings)
{
}

void WAVPlayer::play()
{
    dbg() << "playing WAV file";
}
