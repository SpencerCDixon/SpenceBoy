//
// Created by Spencer Dixon on 5/26/20.
//

#pragma once

#include <SD/Assertions.h>
#include <SD/String.h>
#include <SD/Types.h>

#include <stdio.h>
#include <sys/stat.h>

#define FILE_DEBUG 0

enum class FileMode {
    Read,
    Write,
    // FIXME: Append
};

class File {
public:
    static File open(String path)
    {
        auto result = File(path);
        result.open();
        return result;
    }
    static File open(String path, FileMode mode)
    {
        auto result = File(path);
        result.open(mode);
        return result;
    }

    u64 size_in_bytes()
    {
        struct stat st;
        stat(m_path.characters(), &st);
        return static_cast<u64>(st.st_size);
    }

    String read_all()
    {
        ASSERT(m_file_ptr);
        u64 size = size_in_bytes();
        char* buffer = (char*)calloc(size, sizeof(char));
        fread(buffer, size, 1, m_file_ptr);
        String result = buffer;
        return result;
    }

    bool read_into(char* buffer)
    {
        ASSERT(m_file_ptr);
        u64 size = size_in_bytes();
        int result = fread(buffer, size, 1, m_file_ptr);
        return result == 1;
    }

    u64 write(const String& string)
    {
        ASSERT(m_file_ptr);
        fwrite(string.characters(), sizeof(char), string.length(), m_file_ptr);
        return 0;
    }

    void open(const FileMode& mode = FileMode::Read)
    {
        const char* open_mode;
        switch (mode) {
        case FileMode::Read:
            open_mode = "r";
            break;
        case FileMode::Write:
            open_mode = "w";
            break;
        default:
            ASSERT_NOT_REACHED();
        }
        m_file_ptr = fopen(m_path.characters(), open_mode);
        perror_exit_if(m_file_ptr == nullptr, "File::open()");
    }

    ~File()
    {
        if (m_file_ptr)
            fclose(m_file_ptr);
    }

private:
    File(String path)
        : m_path(path)
    {
    }

private:
    String m_path;
    FILE* m_file_ptr { nullptr };
};
