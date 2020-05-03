cmake_minimum_required(VERSION 3.16)

project(gameboy)

set(CMAKE_CXX_STANDARD 14)

add_executable(gameboy Emulator/main.cpp)
target_include_directories(gameboy PUBLIC ".")

#-------------
# Dependencies
#-------------

# Configure SDL2
# TODO(scd): Set up for Linux environment
message("Configuring SDL2 for windowing")
find_library(SDL2_LIBRARY sdl2)
set(SDL2_INCLUDE_DIR "/usr/local/include/SDL2")
message("  sdl2 lib path     : ${SDL2_LIBRARY}")
message("  sdl2 include path : ${SDL2_INCLUDE_DIR}")

target_include_directories(gameboy PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(gameboy ${SDL2_LIBRARY})

#---------
# Testing
#---------

enable_testing()
add_subdirectory(Tests)
