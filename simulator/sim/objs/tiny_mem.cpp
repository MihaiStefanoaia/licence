//
// Created by mihai-laptop on 5/15/23.
//

#include <iostream>
#include "tiny_mem.h"

namespace sim {
    namespace objs {
        void tiny_mem::eval() {
            if(!RST.get_content()){
                std::cout << "mem: resetting\n";
                for(int i = 0; i < 16; i++)
                    data[i] = 0;
                return;
            }
            switch (state) {
                case IDLE:
//                    std::cout << "mem: waiting\n";
                    ready.set_content(false);
                    if(active.get_content()){
                        state = rw.get_content() ? WRITE_START : READ_START;
                    }
                    break;
                case READ_START:
                    std::cout << "mem: read started\n";
                    addr_buf = 0;
                    for(int i = 0; i < 4; i++)
                        addr_buf |= ((u_int8_t)addr_i[i].get_content() << i);
                    data_buf = data[addr_buf];
                    state = WAIT;
                    next_state = READ_FINISH;
                    break;
                case WAIT: //simply a 1 clock delay
                    std::cout << "mem: waiting...\n";
                    state = next_state;
                    break;
                case READ_FINISH:
                    std::cout << "mem: read finished\n";
                    for(int i = 0; i < 4; i++)
                        data_o[i].set_content(data_buf & (1<<i));
                    ready.set_content(true);
                    state = IDLE;
                    break;
                case WRITE_START:
                    std::cout << "mem: write started\n";
                    data_buf = 0;
                    for(int i = 0; i < 4; i++)
                        data_buf |= ((u_int8_t)data_i[i].get_content() << i);
                    addr_buf = 0;
                    for(int i = 0; i < 4; i++)
                        addr_buf |= ((u_int8_t)addr_i[i].get_content() << i);
                    state = WAIT;
                    next_state = WRITE_FINISH;
                    break;
                case WRITE_FINISH:
                    std::cout << "mem: write finished\n";
                    data[addr_buf] = data_buf;
                    ready.set_content(true);
                    state = IDLE;
                    break;
            }
        }

        void tiny_mem::flag_for_eval(sim::triggering *ev) {
            flag = (ev == &CLK) ? ev : flag;
        }

        tiny_mem::tiny_mem(const bit_array &addr_i, const bit_array &data_i, const bit_array &data_o, bit &active, bit &rw, bit &ready, bit &CLK, bit&RST) :
        addr_i(addr_i), data_i(data_i), data_o(data_o), active(active), rw(rw), ready(ready),CLK(CLK), RST(RST){
            CLK.add_trigger(this,triggering::NEGATIVE);
        }
    } // sim
} // objs