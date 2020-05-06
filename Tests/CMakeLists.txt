add_executable(sd_assert_test ./TestAssertions.cpp)
add_test(assert sd_assert_test)
target_include_directories(sd_assert_test PUBLIC "..")

add_executable(cpu_test ./TestCPU.cpp ../Emulator/CPU.cpp ../SD/LogStream.cpp)
add_test(cpu cpu_test)
target_include_directories(cpu_test PUBLIC "..")
