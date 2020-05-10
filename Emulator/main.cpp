#include "Emulator.h"

int main()
{
    Emulator emulator;

    emulator.init();
    // TODO: load an actual rom to emulate
    //    emulator.load_rom();
    emulator.run();

    return 0;
}
