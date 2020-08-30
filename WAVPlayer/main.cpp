#include <SD/LogStream.h>

#include "RuntimeSettings.h"
#include "WAVPlayer.h"

void print_usage()
{
    dbg() << "Usage: wavplayer --file <path to sound file>\n\n"
          << "\t--asset-dir <location of graphical assets>\n\n";
}

int main(int argc, char* argv[])
{
    // TODO: Fix runtime settings
    RuntimeSettings settings { "./Asssets" };
    String audio_file;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage();
            exit(1);
        }

        if (strcmp(argv[i], "--asset-dir") == 0)
            settings.assets_dir = argv[i + 1];
        if (strcmp(argv[i], "--file") == 0)
            audio_file = argv[i + 1];
    }

    String path("/home/spence/code/games/SpenceBoy/Assets/sound/collect.wav");
    WAVFile::create(path);
//    if (file.is_none()) {
//        dbg() << "unable to create WAVFile from: " << audio_file;
//        ::exit(1);
//    }
//
    WAVPlayer player(settings);
//    player.print_devices();
    player.play_complex(path.characters());

    return 0;
}
