project(game_backend_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

include_directories("../src")

add_executable(game_backend_test "catch.hpp" "tst_game_backend.cpp" "../src/input.cpp" "../src/game_backend.cpp")
add_test(game_backend_test COMMAND game_backend_test)

