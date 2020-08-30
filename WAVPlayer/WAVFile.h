//
// Created by spence on 8/29/20.
//

#pragma once

#include <SD/Option.h>
#include <SD/String.h>
#include <SD/Types.h>
#include <SD/LogStream.h>

#include "InternalSDL.h"

struct WAVAudioData {
    void* audio_data;
    u32 audio_data_size;

    bool is_valid()
    {
        return audio_data && audio_data_size > 0;
    }
};

struct WAVHeader {
    u32 riff_id;
    u32 size;
    u32 wave_id;
};

struct WAVFmtTag {
    u16 w_format_tag;
    u16 num_channels;
    u32 samples_per_second;
    u32 average_bytes_per_second;
    u16 block_align;
    u16 bits_per_sample;
    u16 cb_size;
    u16 valid_bits_per_sample;
    u32 dw_channel_mask;
    u8 sub_format[16];
};

struct WAVChunk {
    u32 id;
    u32 size;
};


class WAVFile {
public:
    static WAVFile* create(const String& path);
    WAVFile(String path, WAVAudioData data, WAVFmtTag* fmt, char* file_contents);
    ~WAVFile()
    {
        dbg() << "freeing contetns";
        free(m_contents);
    }

    u8* audio_data_start() { return (u8*)m_metadata.audio_data; }
    u32 audio_data_length() { return m_metadata.audio_data_size; }
    int frequency() { return (int)m_fmt->samples_per_second; }
    u16 sample_count() { return (u16)(m_metadata.audio_data_size / m_fmt->bits_per_sample); }
    u8 num_channels() { return (u8)m_fmt->num_channels; }
    String path() { return m_path; }
    SDL_AudioFormat format() {
        if (m_fmt->bits_per_sample == 8) {
            return AUDIO_U8;
        } else if (m_fmt->bits_per_sample == 16) {
            return AUDIO_S16;
        } else {
            return AUDIO_S16;
        }
    }

    // 8 bit (or lower) WAV files are always unsigned. 9 bit or higher are always signed:

private:
    String m_path;
    WAVAudioData m_metadata;
    WAVFmtTag* m_fmt; // alive for the lifetyme of m_contents
    char* m_contents;
};
