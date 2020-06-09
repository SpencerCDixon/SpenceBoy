//
// Created by Spencer Dixon on 5/6/20.
//

#include <SD/Bytes.h>
#include <SD/File.h>
#include <SD/LogStream.h>
#include <SD/Timer.h>

#include <Emulator/Emulator.h>

class CPUSnapshotTest {
public:
    CPUSnapshotTest(const char* test_name, const char* rom_path, bool should_update_snapshot, bool verbose_logging)
        : m_name(test_name)
        , m_rom_path(rom_path)
        , m_should_update_snapshot(should_update_snapshot)
        , m_verbose_logging(verbose_logging)
        , m_execution_trace(String("Trace:\n------\n"))
    {
        m_emulator = new Emulator(verbose_logging);
        m_emulator->load_rom(rom_path);
    }
    ~CPUSnapshotTest()
    {
        delete m_emulator;
    }

    void run()
    {
        if (m_verbose_logging)
            dbg() << "--> Running test: " << m_name;

        while (!m_emulator->cpu().step().should_halt) {
            // FIXME: for some reason this results in a crash:
            // Incorrect checksum for freed object 0x7fef1ec02f28: probably modified after being freed.
            //            m_execution_trace += to_trace_line(m_cpu.test_state());
            m_execution_trace = m_execution_trace + to_trace_line(m_emulator->cpu().test_state()) + "\n";
        };

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
        String current = current_snapshot();
        String existing = read_existing_snapshot();

        bool failed = current.length() != existing.length();
        failed = current != existing;

        if (failed) {
            dbg() << "[" << RED "FAIL" RESET << "] " << m_name;
            dbg() << YELLOW "EXPECTED: \n" RESET;
            dbg() << existing;
            dbg() << RED "GOT: \n" RESET;
            dbg() << current;
        } else {
            dbg() << "[ " << GREEN "OK" RESET << " ] " << m_name;
        }
    }

    void update_snapshot()
    {
        if (m_verbose_logging)
            dbg() << "--> Updating snapshot for: " << m_rom_path;

        auto file = File::open(snapshot_path(), FileMode::Write);
        file.write(current_snapshot());
    }

    String read_existing_snapshot()
    {
        auto file = File::open(String(snapshot_path()));
        return file.read_all();
    }

    String current_snapshot()
    {
        auto result = String("");
        result = result + m_execution_trace;
        result = result + "\n";
        result = result + to_snapshot(m_emulator->cpu().test_state());
        return result;
    }

    String snapshot_path()
    {
        auto path = String(m_rom_path);
        path += ".snapshot";
        return path;
    }

private:
    const char* m_name;
    const char* m_rom_path;
    bool m_should_update_snapshot;
    bool m_verbose_logging;
    String m_execution_trace;
    Emulator* m_emulator { nullptr };
};

#define TESTCASE_TYPE_NAME(x) TestCase_##x
#define TEST_CASE(name, path)                                                                                 \
    CPUSnapshotTest TESTCASE_TYPE_NAME(name)(#name, "./Tests/data/" #path, should_update_snapshots, verbose); \
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

    // clang-format off
    {
        auto timer = Timer("Test Suite");
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
        TEST_CASE(complex_subroutine, complex-routine.gb)
    }

    // Not ready for prime time yet
//    TEST_CASE(background_text_render, text-render.gb)

// This example takes 10-15 seconds to complete on my machine. Something is up
// because in SameBoy emulator this is instant. Need to investigate perf reasons for
// this at some point.
//
// After some perf testing, it appears that the culprit is strcat and checksum(). When running
// this program in the gameboy binary everything goes very fast (so it's not op handling code).
//    TEST_CASE(dec_jumps, dec-jumps.gb)

// Will cause infinite loop since we need to enter commands to exit the program.
//        TEST_CASE(joypad_inputs, joypad.gb)


    // clang-format on

    return 0;
}