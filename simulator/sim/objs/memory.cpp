//
// Created by mihai-laptop on 3/28/23.
//

#include "memory.h"

namespace sim{
    namespace objs{
        void memory::flag_for_eval(triggering* to_flag) {
            if(flag == &CLK)
                flag = to_flag;
        }

        void memory::eval() {
            flag = nullptr;
            if(!req_enable.get_content()){
                ready.set_content(false);
                for(int i = 0; i < 8; i++){
                    data_o[i].set_content(false);
                }
                return;
            }

            switch(state){
                case LISTENING: {
                    ready.set_content(false);
                    if(!req_enable.get_content()){
                        break;
                    }
                    u_int32_t input_addr = 0;
                    for (int i = 0; i < 16; i++) {
                        input_addr *= 2;
                        input_addr |= addr_i[15 - i].get_content();
                    }
                    if (rw.get_content()) { //write
                        if(input_addr < 0x0100)
                            return;
                        u_int8_t val_in = 0;
                        for (int i = 0; i < 8; i++) {
                            val_in *= 2;
                            val_in |= data_i[7 - i].get_content();
                        }
                        mem[input_addr] = val_in;
                    } else { //read
                        u_int8_t data;
                        if(input_addr == 0x00F5){ // memory limit
                            data = 255;
                        } else {
                            data = mem[input_addr];
                        }
                        for (int i = 0; i < 8; i++) {
                            data_o[i].set_content((data >> i) & 1);
                        }
                    }
                    state = DONE;
                }
                    break;
                case DONE: {
                    ready.set_content(true);
                    if(!req_enable.get_content())
                        state = LISTENING;
                }
                break;
            }
        }

        memory::memory(const bit_array &addr_i, const bit_array &data_i, bit &rw, bit &req_enable, bit &clk,
                       const bit_array &data_o, bit &ready) : addr_i(addr_i), data_i(data_i), rw(rw), req_enable(req_enable),
                                                   CLK(clk), data_o(data_o), ready(ready) {
            CLK.add_trigger(this,triggering::NEGATIVE);
        }

        memory *memory::instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                                    nlohmann::json entry) {
            auto& addr_i = *array_db[entry["args"][0]];
            auto& data_i = *array_db[entry["args"][1]];
            auto& rw = *wire_db[entry["args"][2]];
            auto& req_enable = *wire_db[entry["args"][3]];
            auto& clk = *wire_db[entry["args"][4]];
            auto& data_o = *array_db[entry["args"][5]];
            auto& ready = *wire_db[entry["args"][6]];
            return new memory(addr_i,data_i,rw,req_enable,clk,data_o,ready);
        }
    }
}
