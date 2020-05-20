//
// Created by Spencer Dixon on 5/6/20.
//

#include <SD/Assertions.h>
#include <SD/Bytes.h>
#include <SD/LogStream.h>

#include <Emulator/CPU.h>

#include <stdio.h>
#include <stdlib.h>
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
    CPUSnapshotTest(const char* test_name, const char* rom_path, bool should_update_snapshot, bool verbose_logging)
        : m_name(test_name)
        , m_rom_path(rom_path)
        , m_should_update_snapshot(should_update_snapshot)
        , m_cpu(CPU(verbose_logging))
        , m_verbose_logging(verbose_logging)
    {
        m_cpu.load_rom(rom_path);
    }

    void run()
    {
        if (m_verbose_logging)
            dbg() << "--> Running test: " << m_name;

        while (m_cpu.step()) { };

        if (m_should_update_snapshot) {
            update_snapshot();
            dbg() << "[" << YELLOW "UPDATED" RESET << "] " << m_name;
        } else {
            compare_snapshot();
        }
    }

private:
    void compare_snapshot()
    {
        char* current_snapshot = generate_snapshot();
        char* existing_snapshot = read_existing_snapshot();

        size_t current_len = strlen(current_snapshot);
        size_t existing_len = strlen(existing_snapshot);

        bool failed = false;
        failed = current_len != existing_len;

        if (!failed) {
            for (size_t i = 0; i < current_len; ++i) {
                if (current_snapshot[i] != existing_snapshot[i]) {
                    failed = true;
                    break;
                }
            }
        }

        if (failed) {
            dbg() << "[" << RED "FAIL" RESET << "] " << m_name;
            dbg() << YELLOW "EXPECTED: \n" RESET;
            dbg() << existing_snapshot;
            dbg() << RED "GOT: \n" RESET;
            dbg() << current_snapshot;
        } else {
            dbg() << "[ " << GREEN "OK" RESET << " ] " << m_name;
        }

        free(current_snapshot);
        free(existing_snapshot);
    }

    void update_snapshot()
    {
        if (m_verbose_logging)
            dbg() << "--> Updating snapshot for: " << m_rom_path;

        char snapshot_path[128];
        strcpy(snapshot_path, m_rom_path);
        strcat(snapshot_path, ".snapshot");

        if (m_verbose_logging)
            dbg() << "--> Writing snapshot to:   " << snapshot_path;

        FILE* fp;
        fp = fopen(snapshot_path, "wb");
        perror_exit_if(fp == nullptr, "update_snapshot()");

        char* snapshot_buffer = generate_snapshot();
        fwrite(snapshot_buffer, sizeof(char), strlen(snapshot_buffer), fp);
        fclose(fp);

        free(snapshot_buffer);
    }

    // Caller is responsible for freeing when done.
    char* read_existing_snapshot()
    {
        char snapshot_path[128];
        strcpy(snapshot_path, m_rom_path);
        strcat(snapshot_path, ".snapshot");

        FILE* fp;
        fp = fopen(snapshot_path, "rb");
        perror_exit_if(fp == nullptr, "compare_snapshot()");

        char* existing_snapshot = (char*)calloc(512, sizeof(char));
        fread(existing_snapshot, 512, 1, fp);

        return existing_snapshot;
    }

    // Caller is responsible for freeing when done.
    char* generate_snapshot()
    {
        auto state = m_cpu.test_state();
        char* snapshot_buffer = (char*)calloc(512, sizeof(char));
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
            state.io_checksum);

        return snapshot_buffer;
    }

private:
    const char* m_name;
    const char* m_rom_path;
    bool m_should_update_snapshot;
    CPU m_cpu;
    bool m_verbose_logging;
};

#define TESTCASE_TYPE_NAME(x) TestCase_##x
#define TEST_CASE(name, path)                                                                         \
    CPUSnapshotTest TESTCASE_TYPE_NAME(name)(#name, "data/" #path, should_update_snapshots, verbose); \
    TESTCASE_TYPE_NAME(name).run();

//
// Test Suite Flags
//
static bool should_update_snapshots = false;
static bool verbose = false;

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--update-snapshots") == 0)
            should_update_snapshots = true;
        if (strcmp(argv[i], "--verbose") == 0)
            verbose = true;
    }

    dbg() << "\nRunning test suite with options: \n"
          << "  should_update_snapshots " << should_update_snapshots << "\n  verbose " << verbose << "\n";

    TEST_CASE(loop, loop.gb)
    TEST_CASE(ram_access, ram.gb)
    TEST_CASE(smily_rendering, smiley.gb)
    TEST_CASE(loading_into_registers, loading.gb)
    TEST_CASE(complement_a_reg_bits, complement.gb)
    TEST_CASE(incrementing_registers, increments.gb)
    TEST_CASE(shifting_and_rotating_bits, shift-rotate.gb)
    TEST_CASE(bit_masks, bit-masks.gb)
    TEST_CASE(stack, stack.gb)
    TEST_CASE(pushing_and_popping, push-pop.gb)

    // Not ready for prime time yet
//    TEST_CASE(background_text_render, text-render.gb)

    return 0;
}