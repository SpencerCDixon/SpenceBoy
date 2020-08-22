#include "InterruptFlags.h"

u16 jump_vector_for(InterruptFlags flag) {
    switch (flag) {
    case InterruptFlags::None:
        return 0;
    case InterruptFlags::VBlank:
        return 0x40;
    case InterruptFlags::LCDStat:
        return 0x48;
    case InterruptFlags::Timer:
        return 0x50;
    case InterruptFlags::Serial:
        return 0x58;
    case InterruptFlags::Joypad:
        return 0x60;
    }
}
