cmake_minimum_required(VERSION 3.16)

project(gameboy)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wimplicit-fallthrough -Wall -Werror)

add_executable(gameboy
        Emulator/main.cpp
        SD/LogStream.cpp
        SD/LogStream.h
        Emulator/CPU.cpp
        Emulator/CPU.h
        SD/Bytes.h
        Emulator/PPU.cpp
        Emulator/PPU.h
        Emulator/Emulator.cpp
        Emulator/Emulator.h
        SD/Option.h
        Emulator/OpCode.h
        )
target_include_directories(gameboy PUBLIC ".")

#-------------
# Dependencies
#-------------

# Configure SDL2
# TODO(scd): Set up for Linux environment

if (APPLE)
    message("Configuring SDL2 for windowing")
    find_library(SDL2_LIBRARY sdl2)
    set(SDL2_INCLUDE_DIR "/usr/local/include/SDL2")
    message("  sdl2 lib path     : ${SDL2_LIBRARY}")
    message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")
endif()

if (NOT APPLE)
    find_package (Threads)
    target_link_libraries (gameboy ${CMAKE_THREAD_LIBS_INIT})

    message("Configuring SDL2 for windowing")
    find_library(SDL2 sdl2)
    set(SDL2_INCLUDE_DIR "/usr/include/SDL2/")
    set(SDL2_LIBRARY "/usr/lib/libSDL2.a")
    message("  sdl2 lib path     : ${SDL2_LIBRARY}")
    message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")

    target_link_libraries(gameboy dl)
endif()

target_include_directories(gameboy PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(gameboy ${SDL2_LIBRARY})

#---------
# Testing
#---------

add_executable(cpu_test ./Tests/TestCPU.cpp ./Emulator/CPU.cpp ./SD/LogStream.cpp)
target_include_directories(cpu_test PUBLIC ".")

