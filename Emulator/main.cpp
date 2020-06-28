//
// Created by Spencer Dixon in 2020.
//

#include "Emulator.h"
#include "RuntimeSettings.h"
#include <SD/LogStream.h>

void print_usage()
{
    dbg() << "Usage: SpenceBoy --rom <pah to rom>\n\n"
          << "\tSpenceBoy is a cross-platform GameBoy Color emulator built for"
          << " pedagogical purposes.\n\n"
          << "\t--verbose - enable verbose logging for easier debugging\n"
          << "\t--asset-dir <location of graphical assets>\n"
          << "\t--help - print this screen\n"
          << "\t--test - run in test mode. Will run through all CPU cycles until a 'halt' is reached\n\n"
          << "\t--debug - run in debug mode allowing step and breakpoint debugging\n\n"
          << "\t--rom - path to ROM to run\n\n";
}

int main(int argc, char* argv[])
{
    RuntimeSettings settings {
        RenderingBackend::SDL,
        false,
        false,
        false,
        "./Asssets"
    };

    String rom_path;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage();
            exit(1);
        }

        if (strcmp(argv[i], "--asset-dir") == 0)
            settings.assets_dir = argv[i + 1];
        if (strcmp(argv[i], "--test") == 0)
            settings.in_test_mode = true;
        if (strcmp(argv[i], "--debug") == 0)
            settings.in_debug_mode = true;
        if (strcmp(argv[i], "--verbose") == 0)
            settings.verbose_logging = true;
        if (strcmp(argv[i], "--rom") == 0)
            rom_path = argv[i + 1];
    }

    if (!settings.in_test_mode) {
        dbg() << "\n\nRunning SpenceBoy with the following settings:\n\n"
              << "\tasset-dir: " << settings.assets_dir << "\n"
              << "\tin-test-mode: " << settings.in_test_mode << "\n"
              << "\tin-debug-mode: " << settings.in_debug_mode << "\n"
              << "\tverbose: " << settings.verbose_logging << "\n";
    }

    Emulator emulator { settings };
    emulator.load_rom(rom_path.characters());
    emulator.run();

    return 0;
}
