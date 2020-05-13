//
// Created by Spencer Dixon on 5/6/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>
#include <SD/Bytes.h>

#include <Emulator/CPU.h>

#include <stdio.h>
#include <string.h>

/*

 Test Phases
 -----------

 Phase 1: Snapshot Testing Final State

 Have the ability to run a ROM and save the CPU & memory state to an output file. Subsequent runs
 can then compare results with that output. This won't catch errors for in between steps but it
 can act as a canary that some refactor I did broke things.

 Phase 2: Tracing

 Full trace of steps the CPU & PPU is making.

 */

// TODO: Maybe a verbose flag which let's us see each step
class CPUSnapshotTest {
public:
    CPUSnapshotTest(const char* rom_path, bool should_update_snapshot)
        : m_rom_path(rom_path)
        , m_should_update_snapshot(should_update_snapshot)
        , m_cpu({})
    {
        m_cpu.load_rom(rom_path);
    }

    void run()
    {
        dbg() << "Running test: " << m_rom_path;
        while (m_cpu.step())
            ;

        if (m_should_update_snapshot)
            update_snapshot();
    }

    void update_snapshot()
    {
        dbg() << "Updating snapshot for: " << m_rom_path;

        char snapshot_path[128];
        strcpy(snapshot_path, m_rom_path);
        strcat(snapshot_path, ".snapshot");

        dbg() << "Writing snapshot to:   " << snapshot_path;

        FILE* fp;
        fp = fopen(snapshot_path, "wb");
        perror_exit_if(fp == NULL, "update_snapshot()");

        // TODO: extract this into reusable register related function?
        auto state = m_cpu.test_state();
        char snapshot_buffer[512];
        sprintf(
            snapshot_buffer,
            "Registers:\n"
            "–---------\n\n"
            "a: %03u [$%02x]   f: %03u [$%02x]\n"
            "b: %03u [$%02x]   c: %03u [$%02x]\n"
            "d: %03u [$%02x]   e: %03u [$%02x]\n"
            "h: %03u [$%02x]   l: %03u [$%02x]\n"
            "s: %03u [$%02x]   p: %03u [$%02x]\n\n"
            "Checksums:\n"
            "–---------\n\n"
            "WRAM Checksum: %llu\n"
            "VRAM Checksum: %llu\n"
            "IO   Checksum: %llu\n",
            state.registers.a,
            state.registers.a,
            state.registers.f,
            state.registers.f,
            state.registers.b,
            state.registers.b,
            state.registers.c,
            state.registers.c,
            state.registers.d,
            state.registers.d,
            state.registers.e,
            state.registers.e,
            state.registers.h,
            state.registers.h,
            state.registers.l,
            state.registers.l,
            state.registers.stack_ptr,
            state.registers.stack_ptr,
            state.registers.program_counter,
            state.registers.program_counter,
            state.wram_checksum,
            state.vram_checksum,
            state.io_checksum
            );

        fwrite(snapshot_buffer, sizeof(char), strlen(snapshot_buffer), fp);
        fclose(fp);
    }

private:
    const char* m_rom_path;
    bool m_should_update_snapshot;
    CPU m_cpu;
};

//const char* path = "./data/loop.gb";
//const char* path = "./data/ram.gb";
//const char* path = "./data/smiley.gb";

// TODO:  --updateSnapshot to update all failing snapshots
int main(int argc, char* argv[])
{
    CPUSnapshotTest test("./data/smiley.gb", true);
    test.run();
    return 0;
}