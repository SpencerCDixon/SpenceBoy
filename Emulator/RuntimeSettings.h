//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include <SD/String.h>

enum class RenderingBackend {
    SDL,
    Serenity,
};

struct RuntimeSettings {
    RenderingBackend backend;
    bool in_test_mode;
    bool verbose_logging;
    String test_dir;
    String assets_dir;
};