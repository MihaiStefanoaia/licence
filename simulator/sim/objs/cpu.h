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

#define ARGS_SIZES_CPU {"cpu",{8,8,8,8,8,4,1,1,1,1,8,8,8,8,16,8,1,1}}

#define NOP   0x0
#define MOV   0x1
#define SPC   0x2
#define JFG   0x3
#define INC   0x4
#define DEC   0x5
#define ROT   0x6
#define NOR   0x7
#define AND   0x8
#define ADC   0x9
#define MUL   0xA
#define DIV   0xB
#define CCS   0xC
#define RET   0xD
#define MOV16 0xE
#define ADD16 0xF

namespace sim {
    namespace objs {
        class cpu : protected evaluable{
        protected:
            //inputs
            bit_array P0_i;
            bit_array P1_i;
            bit_array P2_i;
            bit_array P3_i;
            bit_array mem_val_i;
            bit_array interr;
            bit& mem_ready;
            bit& CLK;
            bit& RST;
            bit& CE;

            //outputs
            bit_array P0_o;
            bit_array P1_o;
            bit_array P2_o;
            bit_array P3_o;
            bit_array mem_addr_o;
            bit_array mem_val_o;
            bit& mem_enable;
            bit& mem_rw;

            //internal variables
            //registers
            u_int32_t rgi = 0;  //instruction register
            u_int8_t *rgi_arr = (u_int8_t*)&rgi;
            //8 bit registers
            u_int8_t rga = 0;
            u_int8_t rgb = 0;
            u_int8_t rgc = 0;
            u_int8_t rgd = 0;
            //16 bit registers
            u_int16_t rmx = 0; //memory access
            u_int16_t rsx = 0; //stack top pointer
            u_int16_t rbx = 0; //stack base pointer
            u_int16_t rpx = 0; //program counter
            u_int8_t& rml = ((u_int8_t*)(&rmx))[0]; //8 bit registers derived from the 16 bit registers
            u_int8_t& rmh = ((u_int8_t*)(&rmx))[1];
            u_int8_t& rsl = ((u_int8_t*)(&rsx))[0];
            u_int8_t& rsh = ((u_int8_t*)(&rsx))[1];
            u_int8_t& rbl = ((u_int8_t*)(&rbx))[0];
            u_int8_t& rbh = ((u_int8_t*)(&rbx))[1];
            u_int8_t& rpl = ((u_int8_t*)(&rpx))[0];
            u_int8_t& rph = ((u_int8_t*)(&rpx))[1];
            u_int8_t mem_reg = 0;
            u_int8_t rgf = 0x0F; // flags

            u_int8_t *op1 = nullptr;
            u_int8_t *op2 = nullptr;

            u_int32_t call_arr[256] = {0};
            u_int8_t  call_ptr = 0;
            u_int8_t  err_state = 0;
            enum : u_int8_t {
                NO_ERROR,
                CALL_OVERFLOW,
                CALL_UNDERFLOW,
                OUT_OF_BOUNDS_MEMORY_ACCESS,
                INVALID_INSTRUCTION,
                INVALID_OPERAND
            };

            enum state_enum : u_int8_t {
                INIT_GET_MEM_LIMIT,
                INIT_GET_INT_VEC_0,
                INIT_GET_INT_VEC_1,
                INIT_GET_INT_VEC_2,
                INIT_GET_INT_VEC_3,
                INIT_GET_INT_VEC_4,
                INIT_GET_INT_VEC_5,
                INIT_GET_INT_VEC_6,
                INIT_GET_INT_VEC_7,
                WAITING_FOR_MEM,
                FETCH_0,
                FETCH_0_DONE,
                FETCH_1,
                FETCH_1_DONE,
                FETCH_2,
                DECODE_0,
                DECODE_1,
                EXECUTE,
                ARTIFICIAL_DELAY, // for calculations to take more time
                ERROR
            };
            state_enum state = INIT_GET_MEM_LIMIT;
            state_enum next  = state;
            u_int8_t delay = 0;
            u_int16_t max_addr = 0;
            u_int16_t interr_table[4] = {0};

        private:
            u_int8_t read_byte(bit_array &arr);
            void write_byte(bit_array &arr, u_int8_t val, u_int8_t mask = 0xFF);
        public:
            cpu(const bit_array &p_0_i, const bit_array &p_1_i, const bit_array &p_2_i, const bit_array &p_3_i,
                const bit_array &mem_val_i, const bit_array &interr, bit &mem_ready, bit &clk, bit &rst, bit &ce,
                const bit_array &p_0_o, const bit_array &p_1_o, const bit_array &p_2_o, const bit_array &p_3_o,
                const bit_array &mem_addr_o, const bit_array &mem_val_o, bit &mem_enable, bit &mem_rw);
            void eval() override;

        };

    } // sim
} // objs

#endif //SIMULATOR_CPU_H
