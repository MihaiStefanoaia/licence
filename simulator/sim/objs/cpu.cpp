//
// Created by Mihai - PC on 11/24/2022.
//

#include "cpu.h"

namespace sim {
    namespace objs {
        u_int8_t cpu::read_byte(bit_array &arr) {
            u_int8_t ret = 0;
            for(int i = 0; i < 8; i++)
                ret = (ret << 1) | (arr[i].get_content() ? 1 : 0);
            return ret;
        }

        void cpu::write_byte(bit_array &arr, u_int8_t val, u_int8_t mask) {
            u_int8_t out = val & mask;
            for(int i = 0; i < 8; i++)
                arr[i].set_content(((out & (1 << i)) != 0));
        }

        void cpu::eval() {
            if(RST.get_content()){
                state = INIT_GET_MEM_LIMIT;
                write_byte(P0_o,0);
                write_byte(P1_o,0);
                write_byte(P2_o,0);
                write_byte(P3_o,0);
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
                return;
            }
            switch(state){
            }
        }

        cpu::cpu(const bit_array &p_0_i, const bit_array &p_1_i, const bit_array &p_2_i, const bit_array &p_3_i,
                 const bit_array &mem_val_i, bit &mem_ready, bit &clk, bit &rst, bit &ce,
                 const bit_array &p_0_o, const bit_array &p_1_o, const bit_array &p_2_o, const bit_array &p_3_o,
                 const bit_array &mem_addr_o, const bit_array &mem_val_o, bit &mem_enable, bit &mem_rw) :
                 P0_i(p_0_i), P1_i(p_1_i), P2_i(p_2_i), P3_i(p_3_i), mem_val_i(mem_val_i),
                 mem_ready(mem_ready), CLK(clk), RST(rst), CE(ce), P0_o(p_0_o), P1_o(p_1_o), P2_o(p_2_o),
                 P3_o(p_3_o), mem_addr_o(mem_addr_o),mem_val_o(mem_val_o),mem_enable(mem_enable),mem_rw(mem_rw) {
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
    } // sim
} // objs