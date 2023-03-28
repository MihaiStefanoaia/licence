//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H
#include "../evaluable.h"
#include "bit.h"
#include "byte.h"
#include "word.h"
#include <sys/types.h>


namespace sim {
    namespace objs {
        class cpu : protected evaluable{
        protected:
            //inputs
            byte P0_i;
            byte P1_i;
            byte P2_i;
            byte P3_i;
            word mem_addr_i;
            byte mem_val_i;
            bit& mem_ready;
            bit& CLK;
            bit& RST;
            bit& CE;

            //outputs
            byte P0_o;
            byte P1_o;
            byte P2_o;
            byte P3_o;
            word mem_addr_o;
            byte mem_val_o;
            bit& mem_enable;
            bit& mem_rw;

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
            u_int8_t rgf;

            enum{
                INIT,
                WAITING,
                FETCH_0,
                FETCH_1,
                FETCH_2,
                DECODE_0,
                DECODE_1,
                EXECUTE,
                ARTIFICIAL_DELAY, // for calculations to take more time
                ERROR
            };
            u_int32_t state = INIT;
            u_int32_t next  = state;
            u_int8_t delay;
        public:
            void a(){
                state = FETCH_0;
            }

        };

    } // sim
} // objs

#endif //SIMULATOR_CPU_H
