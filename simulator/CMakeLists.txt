cmake_minimum_required(VERSION 3.25)
project(simulator)

set(CMAKE_PREFIX_PATH "/home/mihai-laptop/Qt5.12.12/5.12.12/gcc_64")
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

include_directories(sim)
include_directories(sim/objs)
include_directories(sim/test)
include_directories(sim/gui)
include_directories(lib)

add_executable(simulator
        sim/objs/and_module.cpp
        sim/objs/and_module.h
        sim/objs/bit.cpp
        sim/objs/bit.h
        sim/objs/bit_array.cpp
        sim/objs/bit_array.h
        sim/objs/bootloader.cpp
        sim/objs/bootloader.h
        sim/objs/button.cpp
        sim/objs/button.h
        sim/objs/byte.cpp
        sim/objs/byte.h
        sim/objs/cpu.cpp
        sim/objs/cpu.h
        sim/objs/memory.cpp
        sim/objs/memory.h
        sim/objs/microcontroller.cpp
        sim/objs/microcontroller.h
        sim/objs/mux2x1.cpp
        sim/objs/mux2x1.h
        sim/objs/mux4x1.cpp
        sim/objs/mux4x1.h
        sim/evaluable.h
        sim/evaluation_list.cpp
        sim/evaluation_list.h
        sim/triggering.cpp
        sim/triggering.h
        main.cpp sim/objs/bit_array.cpp sim/objs/bit_array.h sim/objs/or_module.cpp sim/objs/or_module.h sim/objs/not_module.cpp sim/objs/not_module.h sim/test/memory_test.cpp sim/test/memory_test.h sim/environment.cpp sim/environment.h sim/objs/button.cpp sim/objs/button.h sim/objs/byte.cpp sim/objs/byte.h sim/objs/word.cpp sim/objs/word.h sim/objs/memory.cpp sim/basic_output.cpp sim/basic_input.cpp sim/environment_generator.cpp sim/transpiler.cpp sim/transpiler.h lib/parser.hh lib/parser.cc lib/scanner.cc sim/objs/none.h sim/objs/tiny_cpu.cpp sim/objs/tiny_cpu.h sim/objs/tiny_mem.cpp sim/objs/tiny_mem.h sim/level_promise.cpp sim/level_promise.h sim/objs/led.cpp sim/monitored.cpp sim/monitored.h sim/gui/sim_monitor.cpp sim/gui/sim_monitor.h)

target_link_libraries(simulator Qt5::Widgets)
