#include "Emulator.h"

int main()
{
    Emulator emulator;

    emulator.init();
    emulator.load_rom("../Tests/data/smiley.gb");
    emulator.run();

    return 0;
}
