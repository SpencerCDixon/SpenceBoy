add_executable(sd_assert ./TestAssertions.cpp)
add_test(assert sd_assert)
target_include_directories(sd_assert PUBLIC "..")
