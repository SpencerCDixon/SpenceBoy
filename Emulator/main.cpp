#include "Emulator.h"

int main()
{
    // TODO: parse args and get runtime settings to pass into emulator.
    Emulator emulator { false };

    emulator.init();
    emulator.load_rom("../Tests/data/joypad.gb");
    emulator.run();

    return 0;
}
