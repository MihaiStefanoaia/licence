//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MICROCONTROLLER_H
#define SIMULATOR_MICROCONTROLLER_H

#include "bit.h"
#include "module.h"

namespace sim {
    namespace objs {

        class microcontroller : module{
        private:
            bit* P0_i[8];
            bit* P1_i[8];
            bit* P2_i[8];
            bit* P3_i[8];
            bit* P4_i[8];
            bit* P5_i[8];
            bit* P0_o[8];
            bit* P1_o[8];
            bit* P2_o[8];
            bit* P3_o[8];
            bit* P4_o[8];
            bit* P5_o[8];
            bit* mem[16];
            bit* mem_rw;
            bit* mem_ready;
            bit* int_sources[6];
            bit* CLK;
            bit* RST;
        };

    } // sim
} // objs

#endif //SIMULATOR_MICROCONTROLLER_H
