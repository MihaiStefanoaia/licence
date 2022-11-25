//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H
#include "../evaluable.h"
#include "module.h"
#include "bit.h"
#include <sys/types.h>


namespace sim {
    namespace objs {

        class cpu : protected module{
            //inputs
            bit* P0_i[8];
            bit* P1_i[8];
            bit* P2_i[8];
            bit* P3_i[8];
            bit* P4_i[8];
            bit* P5_i[8];
            bit* mem_addr_i[16];
            bit* mem_val_i[8];
            bit* mem_ready;
            bit* CLK;
            bit* RST;
            bit* CE;

            //outputs
            bit* P0_o[8];
            bit* P1_o[8];
            bit* P2_o[8];
            bit* P3_o[8];
            bit* P4_o[8];
            bit* P5_o[8];
            bit* mem_addr_o[16];
            bit* mem_val_o[8];
            bit* mem_enable;
            bit* mem_rw;

            //internal variables
            //registers
            u_int32_t rgi;  //instruction register
            //8 bit registers
            u_int8_t rga;
            u_int8_t rgb;
            u_int8_t rgc;
            u_int8_t rgd;
            //16 bit registers
            u_int16_t rmx; //memory access
            u_int16_t rsx; //stack top pointer
            u_int16_t rbx; //stack base pointer
            u_int16_t rpx; //program counter
            u_int8_t& rml = ((u_int8_t*)(&rmx))[0]; //8 bit registers derived from the 16 bit registers
            u_int8_t& rmh = ((u_int8_t*)(&rmx))[1];
            u_int8_t& rsl = ((u_int8_t*)(&rsx))[0];
            u_int8_t& rsh = ((u_int8_t*)(&rsx))[1];
            u_int8_t& rbl = ((u_int8_t*)(&rbx))[0];
            u_int8_t& rbh = ((u_int8_t*)(&rbx))[1];
            u_int8_t& rpl = ((u_int8_t*)(&rpx))[0];
            u_int8_t& rph = ((u_int8_t*)(&rpx))[1];
            u_int8_t mem_reg;

            enum{
                INIT,
                FETCH
            };
            u_int32_t state = INIT;
        public:
            void a(){
                state = FETCH;
            }

        };

    } // sim
} // objs

#endif //SIMULATOR_CPU_H
