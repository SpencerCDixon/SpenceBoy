//
// Created by spence on 8/29/20.
//

#include <SD/LogStream.h>
#include <SD/Types.h>
#include <stdio.h>
#include <math.h>

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
    int i, count = SDL_GetNumAudioDevices(0);
    for (i = 0; i < count; ++i) {
        printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }
}

void WAVPlayer::play_simple(const char* file)
{
    // Quick and dirty using high level APIs as an example
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
    SDL_LoadWAV(file, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    if (!success)
        dbg() << "failed to queue audio data";
    SDL_PauseAudioDevice(deviceId, 0); // unpause
    SDL_Delay(3000);

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

struct AudioData {
    u8* position;
    u32 length;
};

void audio_callback(void* userdata, u8* stream, int len)
{
    AudioData* audio = (AudioData*)userdata;
    if (audio->length == 0)
        return;

    u32 length = (u32)len;
    length = (length > audio->length ? audio->length : length);
    SDL_memcpy(stream, audio->position, length);

    audio->position += length;
    audio->length -= length;
}

void WAVPlayer::play_complex(const char* file)
{
    SDL_AudioSpec wavSpec;
    Uint8* wavStart;
    Uint32 wavLength;

    if(SDL_LoadWAV(file, &wavSpec, &wavStart, &wavLength) == NULL)
    {
        dbg() << "Error: file could not be loaded as an audio file.";
    }

    AudioData audio;
    audio.position = wavStart;
    audio.length = wavLength;

    wavSpec.callback = audio_callback;
    wavSpec.userdata = &audio;

    SDL_AudioDeviceID audioDevice;
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audioDevice == 0)
    {
        dbg() << "Error: " << SDL_GetError();
        return;
    }

    SDL_PauseAudioDevice(audioDevice, 0);

    while (audio.length > 0)
    {
        SDL_Delay(100);
    }

    SDL_CloseAudioDevice(audioDevice);
    SDL_FreeWAV(wavStart);
}

void WAVPlayer::print_devices()
{
    /* List Audio Drivers */
    int i;
    int audDvrCnt = SDL_GetNumAudioDrivers();
    printf("SDL_GetNumAudioDrivers(): Found %d Audio Drivers:\n", audDvrCnt);
    for (i = 0; i < audDvrCnt; i++)
        printf("    Audio Driver %d: %s\n", i, SDL_GetAudioDriver(i));

    /* Find Audio Output Devices*/
    int audOutCnt = SDL_GetNumAudioDevices(0);
    printf("SDL_GetNumAudioDevices(0): Found %d Audio Out Devices:\n", audOutCnt);
    for (i = 0; i < audOutCnt; i++)
        printf("    Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));

    /* Find Audio Input Devices*/
    int audInCnt = SDL_GetNumAudioDevices(1);
    printf("SDL_GetNumAudioDevices(1): Found %d Audio In Devices:\n", audInCnt);
    for (i = 0; i < audInCnt; i++)
        printf("    Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 1));
}
