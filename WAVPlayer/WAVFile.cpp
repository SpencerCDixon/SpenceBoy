//
// Created by spence on 8/29/20.
//
#include "WAVFile.h"
#include <SD/Assertions.h>
#include <SD/File.h>
#include <SD/LogStream.h>

// Spec: http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
// RIFF = Resource Interchange File Format

struct WaveHeader {
    u32 riff_id;
    u32 size;
    u32 wave_id;
};

struct WaveFmtTag {
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

struct WaveChunk {
    u32 id;
    u32 size;
};

#define RIFF_CODE(a, b, c, d) (((u32)(a) << 0) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))
constexpr u32 WAVE_PCM = 1;

enum {
    WAVE_Chunk_ID_fmt = RIFF_CODE('f', 'm', 't', ' '),
    WAVE_Chunk_ID_RIFF = RIFF_CODE('R', 'I', 'F', 'F'),
    WAVE_Chunk_ID_WAVE = RIFF_CODE('W', 'A', 'V', 'E'),
    WAVE_Chunk_ID_data = RIFF_CODE('d', 'a', 't', 'a')
};

struct RiffIterator {
    u8* cursor;
    u8* end_cursor;
};

inline RiffIterator parse_chunk_at(void* cursor, void* end_cursor)
{
    RiffIterator result;
    result.cursor = static_cast<u8*>(cursor);
    result.end_cursor = static_cast<u8*>(end_cursor);
    return result;
}
inline RiffIterator next_chunk(RiffIterator iterator)
{
    WaveChunk* current_chunk = (WaveChunk*)iterator.cursor;
    u32 size = (current_chunk->size + 1) & ~1; // If odd, can have 1 byte of padding
    iterator.cursor += sizeof(WaveChunk) + size;
    return iterator;
}
inline bool is_valid(RiffIterator iterator)
{
    return iterator.cursor < iterator.end_cursor;
}
inline void* get_chunk_data(RiffIterator iterator)
{
    void* result = iterator.cursor + sizeof(WaveChunk);
    return result;
}
inline u32 get_chunk_type(RiffIterator iterator)
{
    WaveChunk* current_chunk = (WaveChunk*)iterator.cursor;
    return current_chunk->id;
}

Option<WAVFile> WAVFile::create(const String& path)
{
    dbg() << "opening WAV file at: " << path;
    auto file = File::open(path);

    if (!file.exists())
        return {};

    dbg() << "wav file is size: " << file.size_in_bytes() << " bytes";
    char* contents = (char*)malloc(file.size_in_bytes());
    file.read_into(contents);

    WaveHeader* header = (WaveHeader*)contents;
    // FIXME: return None
    ASSERT(header->riff_id == WAVE_Chunk_ID_RIFF);
    ASSERT(header->wave_id == WAVE_Chunk_ID_WAVE);

    dbg() << "looks like a legit wav file!";
    dbg() << "  wav data size is: " << header->size;

    void *audio_data = nullptr;
    for (
        RiffIterator iter = parse_chunk_at(header + 1, (u8*)header + 1 + header->size - 4);
        is_valid(iter);
        iter = next_chunk(iter)) {
        switch (get_chunk_type(iter)) {
        case WAVE_Chunk_ID_fmt: {
            dbg() << "fmt chunk";
            WaveFmtTag *fmt = (WaveFmtTag*)get_chunk_data(iter);
            dbg() << "  format tag: " << fmt->w_format_tag;
            dbg() << "  samples per sec: " << fmt->samples_per_second;
            dbg() << "  channels: " << fmt->num_channels;
            dbg() << "  bits per sample: " << fmt->bits_per_sample;

            // Current constraints
            ASSERT(fmt->w_format_tag == WAVE_PCM);
            ASSERT(fmt->samples_per_second == 44100);
            ASSERT(fmt->num_channels == 1);

            break;
        }
        case WAVE_Chunk_ID_data: {
            dbg() << "data chunk";
            audio_data = get_chunk_data(iter);
            break;
        }
        }
        dbg() << "iteration of chunk";
    }
    free(contents);

    if (!audio_data)
        return {};

    return Option<WAVFile>(WAVFile(path));
}

WAVFile::WAVFile(String path)
    : m_path(path)
{
}
