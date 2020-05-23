#include "Emulator.h"

int main()
{
    Emulator emulator;

    emulator.init();
    emulator.load_rom("../Tests/data/stripes.gb");
    emulator.run();

    return 0;
}
