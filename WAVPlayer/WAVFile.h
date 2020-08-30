//
// Created by spence on 8/29/20.
//

#pragma once

#include <SD/Option.h>
#include <SD/String.h>
#include <SD/Types.h>

class WAVFile {
public:
    static Option<WAVFile> create(const String& path);
    WAVFile(String path);

    String path() { return m_path; }

private:
//    s32 m_sample_count;
//    void* m_data;
    String m_path;
};
