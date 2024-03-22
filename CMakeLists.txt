cmake_minimum_required(VERSION 3.9)
project(Emu)


# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pedantic-errors -g")

add_subdirectory(lib/stack)

include_directories(include)
include_directories(lib/stack/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(emu src/compiler.cpp src/exec.cpp src/emu.cpp)
add_executable(emu_comp src/compiler.cpp src/emu_comp.cpp)
add_executable(emu_exec src/exec.cpp src/emu_exec.cpp)
add_executable(stack_test ALIAS test)