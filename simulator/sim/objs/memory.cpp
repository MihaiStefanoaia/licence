//
// Created by mihai-laptop on 3/28/23.
//

#include <iostream>
#include <fstream>
#include "memory.h"

namespace sim{
    namespace objs{
        void memory::flag_for_eval(triggering* to_flag) {
            flag = to_flag;
        }

        void memory::eval() {
            u_int16_t addr_in = 0;
            for (int i = 0; i < 16; i++) {
                addr_in = (addr_in << 1) | (addr_i[15 - i].get_content() ? 1 : 0);
            }
            u_int8_t data_in = 0;
            for (int i = 0; i < 8; i++) {
                data_in = (data_in << 1) | (data_i[7 - i].get_content() ? 1 : 0);
            }
            u_int8_t data_out = 0;
            for (int i = 0; i < 8; i++) {
                data_out = (data_out << 1) | (data_o[7 - i].get_content() ? 1 : 0);
            }

//            std::cout << "mem: \n" <<
//            "\taddr_i = " << addr_in << "\n" <<
//            "\tdata_i = " << (int)data_in << "\n" <<
//            "\tdata_o = " << (int)data_out<< "\n" <<
//            "\tready  = " << ready.get_content() << "\n" <<
//            "\tenable = " << req_enable.get_content() << "\n" <<
//            "\tr/w  = " << (rw.get_content() ? "write" : "read") << "\n";
            flag = nullptr;

            switch(state){
                case LISTENING: {
                    ready.set_content(false);
                    if(!req_enable.get_content()){
                        break;
                    }
                    u_int16_t input_addr = 0;
                    for (int i = 0; i < 16; i++) {
                        input_addr = (input_addr << 1) | (addr_i[15 - i].get_content() ? 1 : 0);
                    }
                    if (rw.get_content()) { //write
                        if(input_addr < 0x0100)
                            return;
                        u_int8_t val_in = 0;
                        for (int i = 0; i < 8; i++) {
                            val_in = (val_in << 1) | (data_i[7 - i].get_content() ? 1 : 0);
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
                            data_o[i].set_content((data & (1 << i)));
                        }
                    }
                    state = DONE;
                }
                    break;
                case DONE: {
//                    std::cout << "memory done, waiting for the cpu\n";
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
            mem[0x00F7] = 2; //rph
            mem[0x00F6] = 0; //rpl
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

        void memory::bootload(const std::string& path) {
            try {
                std::ifstream file(path, std::ios::binary);
                char buf[1<<16];
                file.readsome(buf,1<<16);
                memcpy(mem,buf,1<<16);
            } catch(std::runtime_error& err){
                std::cout << err.what() << "\n";
            }
        }
    }
}
