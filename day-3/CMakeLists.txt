cmake_minimum_required(VERSION 3.20)
project(day-3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler
set(CMAKE_CXX_COMPILER g++-14)


set(BOOST_ROOT /opt/boost/1.88)

include_directories(
    ${BOOST_ROOT}/include
)

link_directories(
    ${BOOST_ROOT}/lib
)

add_executable(puzzle-1 puzzle-1.cpp)

target_compile_options(puzzle-1 PRIVATE -O1)

target_link_libraries(puzzle-1 PRIVATE boost_system)


add_executable(puzzle-2 puzzle-2.cpp)

target_compile_options(puzzle-2 PRIVATE -O1)

target_link_libraries(puzzle-2 PRIVATE boost_system)
