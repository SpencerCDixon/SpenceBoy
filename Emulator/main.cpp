#include "Emulator.h"

int main()
{
    Emulator emulator;

    emulator.init();
    emulator.load_rom("../Tests/data/joypad.gb");
    emulator.run();

    return 0;
}
