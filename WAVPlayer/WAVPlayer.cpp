//
// Created by spence on 8/29/20.
//

#include <stdio.h>
#include <SD/LogStream.h>

#include "InternalSDL.h"
#include "WAVPlayer.h"

WAVPlayer::WAVPlayer(RuntimeSettings settings)
    : m_settings(settings)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        dbg() << "SDL could not initialize. Error: " << SDL_GetError();
}

void WAVPlayer::play(const WAVFile&)
{
    dbg() << "playing WAV file";
}
