//
// Created by mihai-laptop on 3/28/23.
//

#include "memory.h"

namespace sim{
    namespace objs{
        void memory::flag_for_eval(triggering* to_flag) {
            if(to_flag == &RST || flag == nullptr)
                flag = to_flag;
        }

        void memory::eval() {
            if(flag == &RST || RST.get_content()){
                for(int i = 0; i < 1 << 16; i++) {
                    mem[i] = 0;
                }
            } else if(flag == &CLK) {
                if(!CE.get_content()){
                    ready.set_content(false);
                    for(int i = 0; i < 8; i++){
                        val_o[i].set_content(false);
                    }
                    return;
                }

                switch(state){
                    case LISTENING: {
                        ready.set_content(false);
                        u_int32_t input_addr = 0;
                        for (int i = 0; i < 16; i++) {
                            input_addr *= 2;
                            input_addr &= addr_i[i].get_content();
                        }
                        if (rw.get_content()) { //write
                            u_int8_t val_in = 0;
                            for (int i = 0; i < 8; i++) {
                                val_in *= 2;
                                val_in &= val_i[7-i].get_content();
                            }
                            mem[input_addr] = val_in;
                        } else { //read
                            for (int i = 0; i < 8; i++) {
                                val_o[i].set_content((mem[input_addr] >> i) & 1);
                            }
                        }
                        state = DONE;
                    }
                        break;
                    case DONE: {
                        ready.set_content(true);
                        state = LISTENING;
                    }
                }
            }
            flag = nullptr;
        }

        memory::memory(word& addr_i, byte& val_i, bit& rw, bit& CE, bit& CLK, bit& RST, byte& val_o, bit& ready) :
                addr_i(addr_i), val_i(val_i), rw(rw), CE(CE), CLK(CLK), RST(RST), val_o(val_o), ready(ready){
            int new_expected_level = 0;
            for(int i = 0; i < 16; i++)
                new_expected_level = std::max(new_expected_level, addr_i[i].get_expected_level());
            for(int i = 0; i < 8; i++)
                new_expected_level = std::max(new_expected_level, val_i[i].get_expected_level());
            new_expected_level = std::max(new_expected_level, rw.get_expected_level());
            new_expected_level = std::max(new_expected_level, CE.get_expected_level());
            new_expected_level = std::max(new_expected_level, CLK.get_expected_level());
            new_expected_level = std::max(new_expected_level, RST.get_expected_level());

            set_expected_level(new_expected_level + 1);
            ready.set_expected_level(get_expected_level());
            for(int i = 0; i < 8; i++){
                val_o[i].set_expected_level(get_expected_level());
            }



            CLK.add_trigger(this,triggering::NEGATIVE);
            RST.add_trigger(this,triggering::POSITIVE);
        }
    }
}
