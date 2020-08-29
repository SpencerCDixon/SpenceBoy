//
// Created by spence on 8/29/20.
//
#include "WAVFile.h"
#include <SD/File.h>
#include <SD/LogStream.h>

Option<WAVFile> WAVFile::create(const String& path)
{
    dbg() << "opening WAV file at: " << path;
    auto file = File::open(path);

    if (!file.exists())
        return {};

    return Option<WAVFile>(WAVFile());
}

WAVFile::WAVFile()
{
}
