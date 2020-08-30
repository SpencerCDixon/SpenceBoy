//
// Created by spence on 8/29/20.
//
#include "WAVFile.h"
#include <SD/Assertions.h>
#include <SD/File.h>

// Spec: http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
// RIFF = Resource Interchange File Format

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
    WAVChunk* current_chunk = (WAVChunk*)iterator.cursor;
    u32 size = (current_chunk->size + 1) & ~1; // If odd, can have 1 byte of padding. Always round up to power of 2.
    iterator.cursor += sizeof(WAVChunk) + size;
    return iterator;
}
inline bool is_valid(RiffIterator iterator)
{
    return iterator.cursor < iterator.end_cursor;
}
inline void* get_chunk_data(RiffIterator iterator)
{
    void* result = iterator.cursor + sizeof(WAVChunk);
    return result;
}
inline u32 get_chunk_type(RiffIterator iterator)
{
    WAVChunk* current_chunk = (WAVChunk*)iterator.cursor;
    return current_chunk->id;
}
inline u32 get_chunk_size(RiffIterator iterator)
{
    WAVChunk* current_chunk = (WAVChunk*)iterator.cursor;
    return current_chunk->size;
}

WAVFile* WAVFile::create(const String& path)
{
    dbg() << "opening WAV file at: " << path;
    auto file = File::open(path);

    if (!file.exists())
        return nullptr;

    dbg() << "wav file is size: " << file.size_in_bytes() << " bytes";
    char* contents = (char*)malloc(file.size_in_bytes());
    file.read_into(contents);

    WAVHeader* header = (WAVHeader*)contents;
    // FIXME: return None
    ASSERT(header->riff_id == WAVE_Chunk_ID_RIFF);
    ASSERT(header->wave_id == WAVE_Chunk_ID_WAVE);

    dbg() << "looks like a legit wav file!";
    dbg() << "  wav data size is: " << header->size;

    WAVFmtTag* fmt = nullptr;
    WAVAudioData metadata;

    for (
        RiffIterator iter = parse_chunk_at(header + 1, (u8*)header + 1 + header->size - 4);
        is_valid(iter);
        iter = next_chunk(iter)) {
        switch (get_chunk_type(iter)) {
        case WAVE_Chunk_ID_fmt: {
            dbg() << "fmt chunk";
            fmt = (WAVFmtTag*)get_chunk_data(iter);
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
            metadata.audio_data = get_chunk_data(iter);
            metadata.audio_data_size = get_chunk_size(iter);
            break;
        }
        }
        dbg() << "iteration of chunk";
    }

    // Unable to parse properly
    if (!metadata.is_valid() || !fmt) {
        free(contents);
        return nullptr;
    }

    // Strictly enforce 1 channel for now, keep things simple.
    auto channel_count = fmt->num_channels;
    if (channel_count == 1) {
        // easy case, return samples[0] = data and len = audio_data_size
    } else if (channel_count == 2) {
        ASSERT_NOT_REACHED();
    } else {
        ASSERT_NOT_REACHED();
    }

    WAVFile* wav = new WAVFile(path, metadata, fmt, contents);
    return wav;
}

WAVFile::WAVFile(String path, WAVAudioData data, WAVFmtTag* fmt, char* contents)
    : m_path(path)
    , m_metadata(data)
    , m_fmt(fmt)
    , m_contents(contents)
{
}
