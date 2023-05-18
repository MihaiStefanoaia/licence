cmake_minimum_required(VERSION 3.25)
project(simulator)

set(CMAKE_PREFIX_PATH "/home/mihai-laptop/Qt5.12.12/5.12.12/gcc_64")
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)

include_directories(sim)
include_directories(sim/objs)
include_directories(sim/test)

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
        sim/objs/not_module.cpp
        sim/objs/not_module.h
        sim/objs/or_module.cpp
        sim/objs/or_module.h
        sim/objs/word.cpp
        sim/objs/word.h
        sim/test/memory_test.cpp
        sim/test/memory_test.h
        sim/environment.cpp
        sim/environment.h
        sim/evaluable.h
        sim/evaluation_list.cpp
        sim/evaluation_list.h
        sim/master_clk.cpp
        sim/master_clk.h
        sim/triggering.cpp
        sim/triggering.h
        main.cpp sim/environment_generator.cpp sim/environment_generator.h sim/basic_io.h sim/basic_input.cpp sim/basic_input.h sim/basic_output.cpp sim/basic_output.h sim/objs/led.cpp sim/objs/led.h sim/transpiler.cpp)

target_link_libraries(simulator Qt5::Widgets)
