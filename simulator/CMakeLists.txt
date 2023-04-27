cmake_minimum_required(VERSION 3.23)
project(simulator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-Os")

include_directories(sim)
include_directories(sim/objs)
include_directories(lib)

#find_package(Boost)

add_executable(simulator
        sim/objs/and_module.cpp
        sim/objs/and_module.h
        sim/objs/bit.cpp
        sim/objs/bit.h
        sim/objs/bootloader.cpp
        sim/objs/bootloader.h
        sim/objs/cpu.cpp
        sim/objs/cpu.h
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
        sim/master_clk.cpp
        sim/master_clk.h
        sim/triggering.cpp
        sim/triggering.h
        main.cpp sim/objs/bit_array.cpp sim/objs/bit_array.h sim/objs/or_module.cpp sim/objs/or_module.h sim/objs/not_module.cpp sim/objs/not_module.h sim/test/memory_test.cpp sim/test/memory_test.h sim/environment.cpp sim/environment.h sim/objs/button.cpp sim/objs/button.h sim/objs/byte.cpp sim/objs/byte.h sim/objs/word.cpp sim/objs/word.h sim/objs/memory.cpp sim/basic_output.cpp sim/basic_input.cpp sim/environment_generator.cpp sim/transpiler.cpp sim/transpiler.h lib/parser.hh lib/parser.cc lib/scanner.cc)
