//
// Created by Spencer Dixon in 2020.
//

#include "Emulator.h"
#include "RuntimeSettings.h"
#include <SD/LogStream.h>

void print_usage()
{
    dbg() << "Usage: SpenceBoy <rom-path>\n\n"
          << "\tSpenceBoy is a cross-platform GameBoy Color emulator built for"
          << " pedagogical purposes.\n\n"
          << "\t--verbose - enable verbose logging for easier debugging\n"
          << "\t--asset-dir <location of graphical assets>\n"
          << "\t--test-dir <location of test ROMs>\n"
          << "\t--help - print this screen\n"
          << "\t--run-tests - run ROM tests\n\n";
}

int main(int argc, char* argv[])
{
    RuntimeSettings settings {
        RenderingBackend::SDL,
        false,
        false,
        "./Tests",
        "./Asssets"
    };

    const char* rom_path = argv[1];

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage();
            exit(1);
        }

        if (strcmp(argv[i], "--asset-dir") == 0)
            settings.assets_dir = argv[i + 1];
        if (strcmp(argv[i], "--test-dir") == 0)
            settings.test_dir = argv[i + 1];
        if (strcmp(argv[i], "--run-tests") == 0)
            settings.in_test_mode = true;
        if (strcmp(argv[i], "--verbose") == 0)
            settings.verbose_logging = true;
    }

    dbg() << "\n\nRunning SpenceBoy with the following settings:\n\n"
          << "\ttest-dir: " << settings.test_dir << "\n"
          << "\tasset-dir: " << settings.assets_dir << "\n";

    Emulator emulator { settings };
    emulator.load_rom(rom_path);
    emulator.run();

    return 0;
}
