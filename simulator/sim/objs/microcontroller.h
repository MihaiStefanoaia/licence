//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MICROCONTROLLER_H
#define SIMULATOR_MICROCONTROLLER_H

#include "bit.h"
#include "cpu.h"
#include "memory.h"

namespace sim {
    namespace objs {

        class microcontroller : evaluable{
        private:
            bit* P0_i[8];
            bit* P1_i[8];
            bit* P2_i[8];
            bit* P3_i[8];
            bit* P0_o[8];
            bit* P1_o[8];
            bit* P2_o[8];
            bit* P3_o[8];
            bit* int_sources[4];
            bit* CLK;
            bit* RST;
            cpu* cpu_module;
            memory<16>* mem;
        };

    } // sim
} // objs

#endif //SIMULATOR_MICROCONTROLLER_H
