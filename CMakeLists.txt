cmake_minimum_required(VERSION 3.20)
project(PolynomialLab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_polynomial
    test_polynomial.cpp
    polynomial.cpp
)

target_link_libraries(test_polynomial
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_polynomial)