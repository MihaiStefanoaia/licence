//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H
#include "../evaluable.h"
#include "bit.h"
#include "byte.h"
#include "word.h"
#include "environment.h"
#include "json.hpp"
#include "cpu_monitor.h"
#include <sys/types.h>
#include <set>

#define NAME_CPU "cpu"
#define ARGS_SIZES_CPU {NAME_CPU,{8   ,8   ,8   ,8   ,8   ,1   ,1   ,1   ,1   ,8    ,8    ,8    ,8    ,16   ,8    ,1    ,1}}
#define ARGS_DIR_CPU   {NAME_CPU,{true,true,true,true,true,true,true,true,true,false,false,false,false,false,false,false,false}}

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
    namespace gui{
        class cpu_monitor;
    }
    namespace objs {
        class cpu : public evaluable{
        protected:
            //inputs
            bit_array P0_i;
            bit_array P1_i;
            bit_array P2_i;
            bit_array P3_i;
            bit_array mem_data_i;
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
            bit_array mem_data_o;
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
            u_int8_t rgf = 0b1000; // flags

            u_int8_t *operand_1 = nullptr;
            u_int8_t *operand_2 = nullptr;
            u_int16_t *operand_1_mem = nullptr;
            u_int16_t *operand_2_mem = nullptr;

            u_int32_t call_arr[256] = {0};
            u_int8_t  call_ptr = 0;
            enum errors : u_int8_t {
                NO_ERROR,
                CALL_OVERFLOW,
                CALL_UNDERFLOW,
                OUT_OF_BOUNDS_MEMORY_ACCESS,
                INVALID_INSTRUCTION,
                INVALID_OPERAND,
                ILLEGAL_OPERAND
            };
            errors err_state = NO_ERROR;

            enum state_enum : u_int8_t {
                INIT_GET_MEM_LIMIT,
                INIT_GET_RPL,
                INIT_GET_RPH,
                INIT_DONE,
                WAITING_FOR_MEM,
                FETCH_0,
                DECODE_0,
                FETCH_1,
                DECODE_1,
                FETCH_2,
                EXECUTE,
                ERROR
            };
            state_enum state = INIT_GET_MEM_LIMIT;
            state_enum next_state  = state;
            u_int8_t delay = 0;
            u_int16_t max_addr = 255;
            u_int8_t sign_change = 0;


            //verilog assign-like variables
            u_int8_t opcode = 0;
            u_int8_t op1_8b = 0;
            u_int8_t op2_8b = 0;
            u_int8_t op1_16b = 0;
            u_int8_t op2_16b = 0;
            u_int8_t rgi_7_4 = 0; // used for a lot of things
            bool op1_reg = false;
            bool op1_mem = false;
            bool op1_imm = false;
            bool op2_reg = false;
            bool op2_mem = false;
            bool op2_imm = false;
            bool flag_pass = false;

            //quick ways to get operands
            const std::map<u_int8_t,u_int8_t*> operands_reg_binop =
                    {{0b010001, &rga},
                     {0b010101, &rgb},
                     {0b011001, &rgc},
                     {0b011101, &rgd},
                     {0b100001, &rmh},
                     {0b100101, &rml},
                     {0b101001, &rsh},
                     {0b101101, &rsl},
                     {0b110001, &rbh},
                     {0b110101, &rbl},
                     {0b111001, &rph},
                     {0b111101, &rpl}
                     };
            const std::map<u_int8_t,u_int16_t*> operands_mem_binop =
                    {{0b000010, &rmx},
                     {0b000110, &rsx},
                     {0b001010, &rbx},
                     {0b001110, &rpx}
                    };
            const std::map<u_int8_t,u_int16_t*> operands_reg_16bit =
                    {{0b00, &rmx},
                     {0b01, &rsx},
                     {0b10, &rbx},
                     {0b11, &rpx}
                    };
            const std::map<u_int8_t,u_int8_t> instruction_sizes = {
                    {NOP,1}, {MOV,2}, {SPC,2}, {JFG,3},
                    {INC,1}, {DEC,1}, {ROT,2}, {NOR,2},
                    {AND,2}, {ADC,2}, {MUL,1}, {DIV,1},
                    {CCS,3}, {RET,1}, {MOV16,1}, {ADD16,2}};
            const std::set<u_int8_t> optional_imm = {MOV,NOR,AND,ADC};
            const std::map<errors, std::string> err_str = {
                    {NO_ERROR, "NO_ERROR"},
                    {CALL_OVERFLOW, "CALL_OVERFLOW"},
                    {CALL_UNDERFLOW, "CALL_UNDERFLOW"},
                    {OUT_OF_BOUNDS_MEMORY_ACCESS, "OUT_OF_BOUNDS_MEMORY_ACCESS"},
                    {INVALID_INSTRUCTION, "INVALID_INSTRUCTION"},
                    {INVALID_OPERAND, "INVALID_OPERAND"},
                    {ILLEGAL_OPERAND, "ILLEGAL_OPERAND"}
            };
            const std::map<state_enum, std::string> state_str = {
                    {INIT_GET_MEM_LIMIT,"INIT_GET_MEM_LIMIT"},
                    {INIT_GET_RPL,"INIT_GET_RPL"},
                    {INIT_GET_RPH,"INIT_GET_RPH"},
                    {INIT_DONE,"INIT_DONE"},
                    {WAITING_FOR_MEM,"WAITING_FOR_MEM"},
                    {FETCH_0,"FETCH_0"},
                    {DECODE_0,"DECODE_0"},
                    {FETCH_1,"FETCH_1"},
                    {DECODE_1,"DECODE_1"},
                    {FETCH_2,"FETCH_2"},
                    {EXECUTE,"EXECUTE"},
                    {ERROR,"ERROR"}
            };

            //for monitoring
        private:
            static u_int8_t read_byte(bit_array &arr);
            static void write_byte(bit_array &arr, u_int8_t val, u_int8_t mask = 0xFF);
            static void write_word(bit_array &arr, u_int16_t val);
            void begin_read(u_int16_t addr, state_enum n_state);
            void begin_write(u_int16_t addr, u_int8_t val, state_enum n_state);
            void compute_assigns();
            void execute();
        public:
            cpu(const bit_array &p_0_i, const bit_array &p_1_i, const bit_array &p_2_i, const bit_array &p_3_i,
                const bit_array &mem_val_i, bit &mem_ready, bit &clk, bit &rst, bit &ce,
                const bit_array &p_0_o, const bit_array &p_1_o, const bit_array &p_2_o, const bit_array &p_3_o,
                const bit_array &mem_addr_o, const bit_array &mem_val_o, bit &mem_enable, bit &mem_rw);
            void eval() override;
            static cpu *instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                             nlohmann::json entry);

            void setup_operands();

            void execute_spc();
            friend class sim::gui::cpu_monitor;
        };

    } // sim
} // objs

#endif //SIMULATOR_CPU_H
