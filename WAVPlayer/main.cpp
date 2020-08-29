#include <SD/LogStream.h>

#include "RuntimeSettings.h"
#include "WAVPlayer.h"

void print_usage()
{
    dbg() << "Usage: wavplayer\n\n"
          << "\t--asset-dir <location of graphical assets>\n\n";
}

int main(int argc, char* argv[])
{
    RuntimeSettings settings { "./Asssets" };

    printf("wav player inited\n");

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage();
            exit(1);
        }

        if (strcmp(argv[i], "--asset-dir") == 0)
            settings.assets_dir = argv[i + 1];
    }

    WAVPlayer player(settings);
    player.play();

    return 0;
}
