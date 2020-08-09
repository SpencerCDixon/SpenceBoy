cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(gameboy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wimplicit-fallthrough -Wall -Wextra -Werror)

add_executable(gameboy
        Emulator/CPU.cpp
        Emulator/CPU.h
        Emulator/Debugger.cpp
        Emulator/Debugger.h
        Emulator/Emulator.cpp
        Emulator/Emulator.h
        Emulator/GUI/Bitmap.h
        Emulator/GUI/Rect.h
        Emulator/GUI/SDLRenderer.cpp
        Emulator/GUI/SDLRenderer.h
        Emulator/GUI/Texture.cpp
        Emulator/GUI/Texture.h
        Emulator/IODevice.cpp
        Emulator/IODevice.h
        Emulator/InternalSDL.h
        Emulator/Joypad.cpp
        Emulator/Joypad.h
        Emulator/MMU.cpp
        Emulator/MMU.h
        Emulator/MemoryMap.h
        Emulator/OpCode.h
        Emulator/PPU.cpp
        Emulator/PPU.h
        Emulator/RuntimeSettings.h
        Emulator/SoundCard.cpp
        Emulator/SoundCard.h
        Emulator/main.cpp
        SD/Bytes.h
        SD/File.cpp
        SD/File.h
        SD/LogStream.cpp
        SD/LogStream.h
        SD/Option.h
        SD/String.cpp
        SD/Timer.h
        SD/Vector.h
        Emulator/InterruptFlags.h)
target_include_directories(gameboy PUBLIC ".")

#---------
# Testing
#---------

add_executable(string_test Tests/TestString.cpp SD/String.cpp SD/LogStream.cpp SD/Utility.h)
target_include_directories(string_test PUBLIC ".")

add_executable(joypad_test Tests/TestJoypadDevice.cpp SD/LogStream.cpp Emulator/Joypad.cpp Emulator/GUI/SDLRenderer.cpp Emulator/GUI/Texture.cpp SD/String.cpp)
target_include_directories(joypad_test PUBLIC ".")

add_executable(vector_test Tests/TestVector.cpp SD/LogStream.cpp SD/String.cpp)
target_include_directories(vector_test PUBLIC ".")

#-------------
# Dependencies
#-------------

if (APPLE)
    message("Configuring SDL2 for windowing on OSX")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

    find_library(SDL2_LIBRARY sdl2)
    find_package(SDL2_image REQUIRED)
    set(SDL2_INCLUDE_DIR "/usr/local/include/SDL2")
    message("  sdl2 lib path     : ${SDL2_LIBRARY}")
    message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(gameboy ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})
    target_link_libraries(joypad_test ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})
endif()

if (NOT APPLE)
    message("Configuring SDL2 for windowing on Linux")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    message("  sdl2 lib path     : ${SDL2_LIBRARY}")
    message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(gameboy ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY})
    target_link_libraries(joypad_test ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY})
endif()

