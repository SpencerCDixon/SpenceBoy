//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include <SD/String.h>

enum class RenderingBackend {
    SDL,
    Serenity,
};

// TODO:
// * --backend <serenity|sdl|gtk|metal>
struct RuntimeSettings {
    RenderingBackend backend;
    bool in_test_mode;
    bool verbose_logging;
    bool in_debug_mode;
    String assets_dir;
};