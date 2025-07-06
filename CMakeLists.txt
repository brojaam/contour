cmake_minimum_required(VERSION 3.10)
project(ContourProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ContourLib
    src/MathUtils.cpp
    src/LineSegment.cpp
    src/ArcSegment.cpp
    src/Contour.cpp
)

add_executable(ContourProject src/main.cpp)
target_link_libraries(ContourProject PRIVATE ContourLib)

enable_testing()
add_executable(ContourTests tests/ContourTests.cpp)
target_link_libraries(ContourTests PRIVATE ContourLib gtest gtest_main)
add_test(NAME ContourTests COMMAND ContourTests)