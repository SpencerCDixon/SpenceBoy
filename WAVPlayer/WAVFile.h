//
// Created by spence on 8/29/20.
//

#pragma once

#include <SD/Option.h>
#include <SD/String.h>

class WAVFile {
public:
    static Option<WAVFile> create(const String& path);
    WAVFile();
};
