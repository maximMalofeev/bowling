project(bowling_test LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

include_directories("../src")

add_executable(tst_input "tst_input.cpp" "../src/input.cpp")
add_test(tst_input COMMAND tst_input)
target_link_libraries(tst_input PRIVATE Qt5::Test)
