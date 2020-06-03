cmake_minimum_required(VERSION 3.16)

project(gameboy)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wimplicit-fallthrough -Wall -Wextra -Werror)

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
        Emulator/Buffer.h
        SD/String.cpp
        SD/File.cpp SD/File.h Emulator/Input.cpp Emulator/Input.h)
target_include_directories(gameboy PUBLIC ".")

#-------------
# Dependencies
#-------------

if (APPLE)
    message("Configuring SDL2 for windowing on OSX")
    find_library(SDL2_LIBRARY sdl2)
    set(SDL2_INCLUDE_DIR "/usr/local/include/SDL2")
    message("  sdl2 lib path     : ${SDL2_LIBRARY}")
    message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(gameboy ${SDL2_LIBRARY})
endif()

if (NOT APPLE)
    message("Configuring SDL2 for windowing on Linux")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    find_package(SDL2 REQUIRED)
    message("  sdl2 lib path     : ${SDL2_LIBRARY}")
    message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(gameboy ${SDL2_LIBRARIES})
endif()



#---------
# Testing
#---------

add_executable(cpu_test ./Tests/TestCPU.cpp ./Emulator/CPU.cpp ./SD/LogStream.cpp ./SD/String.cpp SD/File.cpp SD/File.h)
target_include_directories(cpu_test PUBLIC ".")

add_executable(string_test ./Tests/TestString.cpp ./SD/String.cpp ./SD/LogStream.cpp SD/Utility.h)
target_include_directories(string_test PUBLIC ".")

