//
// Created by Mihai - PC on 11/24/2022.
//

#include <iostream>
#include "cpu.h"

namespace sim {
    namespace objs {
        u_int8_t cpu::read_byte(bit_array &arr) {
            u_int8_t ret = 0;
            for(int i = 0; i < 8; i++)
                ret = (ret << 1) | (arr[7 - i].get_content() ? 1 : 0);
            return ret;
        }

        void cpu::write_byte(bit_array &arr, u_int8_t val, u_int8_t mask) {
            for(int i = 0; i < 8; i++)
                if(mask & (1 << i))
                    arr[i].set_content(((val & (1 << i)) != 0));
        }

        void cpu::eval() {
            if(RST.get_content()){
                state = INIT_GET_MEM_LIMIT;
                write_byte(P0_o,0);
                write_byte(P1_o,0);
                write_byte(P2_o,0);
                write_byte(P3_o,0);
                write_byte(mem_data_o, 0);
                write_word(mem_addr_o, 0);
                rga = 0;
                rgb = 0;
                rgc = 0;
                rgd = 0;
                rmx = 0;
                rsx = 0;
                rbx = 0;
                rpx = 0;
                rgi = 0;
                mem_reg = 0;
                max_addr = 255;
                return;
            }

            if(!CE.get_content() || !CLK.get_content())
                return;

            switch(state){
                case INIT_GET_MEM_LIMIT:
                    begin_read(0x00F5,INIT_GET_RPL);
                    break;
                case INIT_GET_RPL:
                    max_addr |= ((u_int16_t)mem_reg) << 8; // the data from the previous instruction
                    rbx = max_addr;
                    rsx = rbx;

                    begin_read(0x00F6, INIT_GET_RPH);
                    break;
                case INIT_GET_RPH:
                    rpl = mem_reg;

                    begin_read(0x00F7, INIT_DONE);
                    break;
                case INIT_DONE:
                    rph = mem_reg;

                    state = FETCH_0;
                    break;
                case WAITING_FOR_MEM:
                    std::cout << "waiting... " << mem_enable.get_content() << " " << mem_rw.get_content() << "\n";
                    if(mem_ready.get_content()){
                        std::cout << "mem ready is set\n";
                        if(!mem_rw.get_content())
                            mem_reg = read_byte(mem_data_i);
                        mem_enable.set_content(false);

                        state = next_state;
                    }
                    break;
                case FETCH_0:
                    rgi = 0;
                    begin_read(rpx,DECODE_0);
                    rpx++;
                    break;
                case DECODE_0:
                    rgi_arr[0] = mem_reg;
                    compute_assigns();

                    //invalid operands, first byte
                    switch(opcode){
                        case ADC:
                        case AND:
                        case NOR:
                            if(!op1_reg) // if the left operand is a memory access or immediate
                                err_state = ILLEGAL_OPERAND;
                        case MOV: //this check also applies to the above instructions
                            if(op1_16b == 0 || op2_16b == 0 || op1_imm ||
                              (op1_16b == op2_16b && (!op1_reg))) // if both operands are the same kind they need to be registers
                                err_state = ILLEGAL_OPERAND;
                            break;
                        case SPC:
                            if(rgi_7_4 >= 8){
                                err_state = INVALID_INSTRUCTION;
                            }
                            break;
                        case ROT:
                            if(rgi_7_4 < 0b0100)
                                err_state = INVALID_OPERAND;
                            break;
                    }
                    if(err_state != NO_ERROR){
                        state = ERROR;
                        break;
                    }

                    if(instruction_sizes.at(opcode) == 1)
                        state = EXECUTE;
                    else
                        state = FETCH_1;
                    break;
                case FETCH_1:
                    begin_read(rpx,DECODE_1);
                    rpx++;
                    break;
                case DECODE_1:
                    rgi_arr[1] = mem_reg;
                    compute_assigns();
                    // error checking
                    switch(opcode){
                        case MOV:
                        case NOR:
                        case AND:
                        case ADC:
                            if(!(operands_reg_binop.count(op1_8b) || operands_mem_binop.count(op1_8b)) ||
                               !(operands_reg_binop.count(op2_8b) || operands_mem_binop.count(op2_8b) || op2_imm)){
                                err_state = INVALID_OPERAND;
                            }
                            break;
                    }

                    if(err_state != NO_ERROR){
                        state = ERROR;
                        break;
                    }

                    setup_operands();

                    if(instruction_sizes.at(opcode) == 3 ||
                      (instruction_sizes.at(opcode) == 2 && optional_imm.count(opcode) && op2_imm))
                        state = FETCH_2;
                    else if(op2_mem){
                        if(*operand_2_mem > max_addr){
                            state = ERROR;
                            err_state = OUT_OF_BOUNDS_MEMORY_ACCESS;
                            break;
                        }
                        begin_read(*operand_2_mem,EXECUTE);
                    } else{
                        state = EXECUTE;
                    }
                    break;
                case FETCH_2:
                    begin_read(rpx,EXECUTE);
                    rpx++;
                    break;
                case EXECUTE:
                    rgi_arr[2] = mem_reg;
                    compute_assigns();
                    execute();
                    break;
                case ERROR:
                    write_byte(P0_o,err_state);
                    write_byte(P2_o,rpl);
                    write_byte(P3_o,rph);
                    state = ERROR;
                    break;
            }
        }

        cpu::cpu(const bit_array &p_0_i, const bit_array &p_1_i, const bit_array &p_2_i, const bit_array &p_3_i,
                 const bit_array &mem_val_i, bit &mem_ready, bit &clk, bit &rst, bit &ce,
                 const bit_array &p_0_o, const bit_array &p_1_o, const bit_array &p_2_o, const bit_array &p_3_o,
                 const bit_array &mem_addr_o, const bit_array &mem_val_o, bit &mem_enable, bit &mem_rw) :
                P0_i(p_0_i), P1_i(p_1_i), P2_i(p_2_i), P3_i(p_3_i), mem_data_i(mem_val_i),
                mem_ready(mem_ready), CLK(clk), RST(rst), CE(ce), P0_o(p_0_o), P1_o(p_1_o), P2_o(p_2_o),
                P3_o(p_3_o), mem_addr_o(mem_addr_o), mem_data_o(mem_val_o), mem_enable(mem_enable), mem_rw(mem_rw) {
            CLK.add_trigger(this,triggering::POSITIVE);
        }

        cpu *cpu::instantiate(std::map<std::string, bit*> &wire_db, std::map<std::string, bit_array*> &array_db, nlohmann::json entry) {
            auto& p_0_i = *array_db[entry["args"][0]];
            auto& p_1_i = *array_db[entry["args"][1]];
            auto& p_2_i = *array_db[entry["args"][2]];
            auto& p_3_i = *array_db[entry["args"][3]];

            auto& mem_val_i = *array_db[entry["args"][4]];

            auto& mem_ready = *wire_db[entry["args"][5]];
            auto& clk = *wire_db[entry["args"][6]];
            auto& rst = *wire_db[entry["args"][7]];
            auto& ce = *wire_db[entry["args"][8]];

            auto& p_0_o = *array_db[entry["args"][9]];
            auto& p_1_o = *array_db[entry["args"][10]];
            auto& p_2_o = *array_db[entry["args"][11]];
            auto& p_3_o = *array_db[entry["args"][12]];

            auto& mem_addr_o = *array_db[entry["args"][13]];
            auto& mem_val_o = *array_db[entry["args"][14]];
            auto& mem_enable = *wire_db[entry["args"][15]];
            auto& mem_rw = *wire_db[entry["args"][16]];
            return new cpu(p_0_i,p_1_i,p_2_i,p_3_i,mem_val_i, mem_ready, clk,rst,ce,p_0_o,p_1_o,p_2_o,p_3_o,mem_addr_o,mem_val_o,mem_enable,mem_rw);
        }

        void cpu::write_word(bit_array &arr, u_int16_t val) {
            for(int i = 0; i < 8; i++)
                arr[i].set_content(((val & (1 << i)) != 0));
        }

        void cpu::begin_read(u_int16_t addr, cpu::state_enum n_state) {
            write_word(mem_addr_o,addr);
            mem_rw.set_content(false); // read
            mem_enable.set_content(true);
            state = WAITING_FOR_MEM;
            next_state = n_state;
        }

        void cpu::begin_write(u_int16_t addr, u_int8_t val, cpu::state_enum n_state) {
            write_word(mem_addr_o, addr);
            write_byte(mem_data_o, val);
            mem_rw.set_content(true); // write
            mem_enable.set_content(true);
            state = WAITING_FOR_MEM;
            next_state = n_state;
        }

        void cpu::compute_assigns() {
            opcode = rgi_arr[0] & 0x0F; //rgi[3:0]
            op1_8b  = ((rgi_arr[1] & 0x0F) << 2) | ((rgi_arr[0] & 0x30) >> 4); // {rgi[11: 8],rgi[5,4]}
            op2_8b  = ((rgi_arr[1] & 0xF0) >> 2) | ((rgi_arr[0] & 0xC0) >> 6); // {rgi[15:12],rgi[7,6]}
            op1_16b = (rgi_arr[0] & 0x30) >> 4; // rgi[5:4]
            op2_16b = (rgi_arr[0] & 0xC0) >> 6; // rgi[7:6]
            rgi_7_4 = (rgi_arr[0] & 0xF0) >> 4; // self-explanatory
            op1_reg = op1_16b == 0b01;
            op1_mem = op1_16b == 0b10;
            op1_imm = op1_16b == 0b11;
            op2_reg = op2_16b == 0b01;
            op2_mem = op2_16b == 0b10;
            op2_imm = op2_16b == 0b11;
            flag_pass = (rgi_7_4 |  ((rgi_7_4 & 0b1000) ? rgf : ~rgf)) == 0xF;
        }

        void cpu::execute() {
            state = FETCH_0;
            switch(opcode){
                case NOP:
                    break;
                case MOV:
                    if(op1_mem){ // memory location
                        if(*operand_1_mem > max_addr){
                            state = ERROR;
                            err_state = OUT_OF_BOUNDS_MEMORY_ACCESS;
                            break;
                        }
                        begin_write(*operand_1_mem,*operand_2,FETCH_0);
                    } else {
                        *operand_1 = *operand_2;
                    }
                    break;
                case SPC:
                    execute_spc();
                    break;
                case CCS:
                    if(call_ptr == 255){
                        state = ERROR;
                        err_state = CALL_OVERFLOW;
                        break;
                    }
                    if(flag_pass){
                        call_arr[call_ptr]  = 0;
                        call_arr[call_ptr] |= ((int)rpx << 8);
                        call_arr[call_ptr] |= rbx;
                        call_ptr++;
                        rbx = rsx;
                    }

                case JFG:
                    rpl = flag_pass ? rgi_arr[1] : rpl;
                    rph = flag_pass ? rgi_arr[2] : rph;
                    break;
                case INC:
                    rgf = 0b1000;
                    sign_change = *operand_1;

                    (*operand_1)++;

                    sign_change = (sign_change ^ *operand_1) & 0b10000000;
                    if(*operand_1 == 0)
                        rgf |= 0b0001;
                    if(sign_change)
                        rgf |= 0b0100;
                    break;
                case DEC:
                    rgf = 0b1000;
                    sign_change = *operand_1;

                    (*operand_1)--;

                    sign_change = (sign_change ^ *operand_1) & 0b10000000;
                    if(*operand_1 == 0)
                        rgf |= 0b0001;
                    if(sign_change)
                        rgf |= 0b0100;
                    break;
                case ROT:
                {
                    auto val = *operand_1;
                    auto amount = rgi_arr[1] & 0b0111;
                    auto hi = val << amount;
                    auto lo = val >> (8 - amount);
                    *operand_1 = hi | lo;
                }
                break;
                case NOR:
                    rgf = 0b1000;
                    sign_change = *operand_1;

                    *operand_1 = ~((*operand_1) | (*operand_2));

                    sign_change = (sign_change ^ *operand_1) & 0b10000000;
                    if(*operand_1 == 0)
                        rgf |= 0b0001;
                    if(sign_change)
                        rgf |= 0b0100;
                    break;
                case AND:
                    rgf = 0b1000;
                    sign_change = *operand_1;

                    *operand_1 = (*operand_1) & (*operand_2);

                    sign_change = (sign_change ^ *operand_1) & 0b10000000;
                    if(*operand_1 == 0)
                        rgf |= 0b0001;
                    if(sign_change)
                        rgf |= 0b0100;
                    break;
                case ADC:
                    rgf = 0b1000;
                    {
                        int a = (*operand_1) + (*operand_2);
                        if(a >= 256)
                            rgf |= 0b0010;
                    }
                    sign_change = *operand_1;

                    *operand_1 = (*operand_1) + (*operand_2);

                    sign_change = (sign_change ^ *operand_1) & 0b10000000;
                    if(*operand_1 == 0)
                        rgf |= 0b0001;
                    if(sign_change)
                        rgf |= 0b0100;
                    break;
                case MUL:
                {
                    u_int16_t result = rga * rgb;
                    auto *r_arr = (u_int8_t*)&result;
                    rga = r_arr[0];
                    rgb = r_arr[1];
                }
                break;
                case DIV:
                {
                    auto rga_tmp = rga;
                    auto rgb_tmp = rgb;
                    rga = rga_tmp / rgb_tmp;
                    rgb = rga_tmp % rgb_tmp;
                }
                break;
                case RET:
                    if(call_ptr == 0){
                        state = ERROR;
                        err_state = CALL_UNDERFLOW;
                        break;
                    }
                    call_ptr--;
                    rpx = call_arr[call_ptr] >> 8;
                    rsx = rbx;
                    rbx = call_arr[call_ptr] & 0xFFFF;

                    break;
                case MOV16:
                    *operand_1_mem = *operand_2_mem;
                    break;
                case ADD16:
                {
                    u_int16_t value = rgi_arr[1] > 127 ? (u_int16_t)rgi_arr[1] | 0xFF00 : rgi_arr[1];
                    *operand_1_mem = *operand_1_mem + value;
                }
                    break;
            }
        }

        void cpu::setup_operands() {
            switch(opcode){
                case MOV:
                    if(op1_mem){
                        operand_1 = nullptr;
                        operand_1_mem = operands_mem_binop.at(op1_8b);
                    }
                case NOR:
                case AND:
                case ADC:
                    if(op1_reg)
                        operand_1 = operands_reg_binop.at(op1_8b);

                    if(op2_reg)
                        operand_2 = operands_reg_binop.at(op2_8b);
                    else if(op2_mem){
                        operand_2_mem = operands_mem_binop.at(op2_8b);
                        operand_2 = &mem_reg;
                    } else if(op2_imm)
                        operand_2 = &rgi_arr[2];
                    break;
                case INC:
                case DEC:
                case ROT:
                    operand_1 = operands_reg_binop.at((rgi_7_4 << 2) | 0b01);
                    break;
                case MOV16:
                case ADD16:
                    operand_1_mem = operands_reg_16bit.at(op1_16b);
                    operand_2_mem = operands_reg_16bit.at(op2_16b);
                    break;
            }
        }

        void cpu::execute_spc() {
            switch(rgi_7_4){
                case 0:
                    write_byte(P0_o,0,rgi_arr[1]);
                    rga = read_byte(P0_i) & rgi_arr[1];
                    break;
                case 1:
                    write_byte(P0_o,rga,rgi_arr[1]);
                    break;
                case 2:
                    write_byte(P1_o,0,rgi_arr[1]);
                    rga = read_byte(P1_i) & rgi_arr[1];
                    break;
                case 3:
                    write_byte(P1_o,rga,rgi_arr[1]);
                    break;
                case 4:
                    write_byte(P2_o,0,rgi_arr[1]);
                    rga = read_byte(P2_i) & rgi_arr[1];
                    break;
                case 5:
                    write_byte(P2_o,rga,rgi_arr[1]);
                    break;
                case 6:
                    write_byte(P3_o,0,rgi_arr[1]);
                    rga = read_byte(P3_i) & rgi_arr[1];
                    break;
                case 7:
                    write_byte(P3_o,rga,rgi_arr[1]);
                    break;
            }
        }

        void cpu::flag_for_eval(sim::triggering *ev) {
            if(ev == &CLK && CLK.get_content())
                flag = ev;
        }
    } // sim
} // objs