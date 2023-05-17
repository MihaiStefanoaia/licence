//
// Created by mihai-laptop on 5/9/23.
//

#include "tiny_cpu.h"

namespace sim {
    namespace objs {
        tiny_cpu::tiny_cpu(const bit_array& addr_o, const bit_array& data_i, const bit_array& data_o, bit &active, bit &rw, bit &ready, const bit_array& port_i, const bit_array& port_o, bit &interr, bit &CLK, bit &RST)
        : addr_o(addr_o), data_i(data_i), data_o(data_o), active(active), rw(rw), ready(ready), port_i(port_i), port_o(port_o), interr(interr), CLK(CLK), RST(RST) {
            if(addr_o.get_size() < 4 ||
               data_i.get_size() < 4 ||
               data_o.get_size() < 4 ||
               port_i.get_size() < 4 ||
               port_o.get_size() < 4)
                throw std::runtime_error("invalid bit array sizes in generating the tiny_cpu");
            CLK.add_trigger(this,triggering::POSITIVE);
        }

        void tiny_cpu::eval() {
            if(RST.get_content()){
                state=POLL;
                reg = 0;
                for(int i = 0; i < 4; i++){
                    data_o[i].set_content(false);
                    port_o[i].set_content(false);
                }
                return;
            }
            switch(state){
                case POLL:
                    if(interr.get_content()) {
                        state = READ_MEM;
                        reg = 0;
                        for(int i = 0; i < 4; i++){
                            port_o[i].set_content(false);
                            u_int8_t tmp = port_i[i].get_content();
                            reg |= (tmp << i);
                        }
                    } else
                        state = POLL;
                    break;
                case READ_MEM:
                    next_state = WRITE_PORT;
                    state = WAIT_FOR_MEMORY;
                    rw.set_content(false);
                    active.set_content(true);
                    for(int i = 0; i < 4; i++){
                        addr_o[i].set_content((reg & (1 << i)) != 0);
                    }
                    break;
                case WRITE_MEM:
                    next_state = POLL;
                    state = WAIT_FOR_MEMORY;
                    rw.set_content(true);
                    active.set_content(true);
                    for(int i = 0; i < 4; i++){
                        addr_o[i].set_content((reg & (1 << i)) != 0);
                    }
                    break;
                case WAIT_FOR_MEMORY:
                    state = (ready.get_content()) ? next_state : state;
                    active.set_content(!ready.get_content());
                    break;
                case WRITE_PORT:
                    reg++;
                    for(int i = 0; i < 4; i++){
                        port_o[i].set_content(reg & (1<<i));
                    }
                    state = WRITE_MEM;
                    break;
            }
        }

        void tiny_cpu::flag_for_eval(sim::triggering *ev) {
            flag = (ev == &CLK) ? ev : flag;
        }
    } // sim
} // objs