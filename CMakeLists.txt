cmake_minimum_required(VERSION 3.14)
project(TemplatesLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ──────────────────────────────────────────────
#  Google Test (fetched automatically)
# ──────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ──────────────────────────────────────────────
#  Tests (templates are header-only)
# ──────────────────────────────────────────────
enable_testing()

set(STRICT_FLAGS -Wall -Wextra -Wpedantic -Werror)
set(SANITIZER_FLAGS -fsanitize=address,undefined -fno-sanitize-recover=all)

# Array<T>
add_executable(array_tests array_tests.cpp)
target_include_directories(array_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(array_tests PRIVATE GTest::gtest_main)
target_compile_options(array_tests PRIVATE ${STRICT_FLAGS} ${SANITIZER_FLAGS})
target_link_options(array_tests PRIVATE -fsanitize=address,undefined)

# Optional<T>
add_executable(optional_tests optional_tests.cpp)
target_include_directories(optional_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(optional_tests PRIVATE GTest::gtest_main)
target_compile_options(optional_tests PRIVATE ${STRICT_FLAGS} ${SANITIZER_FLAGS})
target_link_options(optional_tests PRIVATE -fsanitize=address,undefined)

# UniquePtr<T, Deleter>
add_executable(unique_ptr_tests unique_ptr_tests.cpp)
target_include_directories(unique_ptr_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(unique_ptr_tests PRIVATE GTest::gtest_main)
target_compile_options(unique_ptr_tests PRIVATE ${STRICT_FLAGS} ${SANITIZER_FLAGS})
target_link_options(unique_ptr_tests PRIVATE -fsanitize=address,undefined)

include(GoogleTest)
gtest_discover_tests(array_tests)
gtest_discover_tests(optional_tests)
gtest_discover_tests(unique_ptr_tests)
