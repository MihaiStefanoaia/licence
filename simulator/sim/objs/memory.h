//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "bit.h"
#include <sys/types.h>

namespace sim {
    namespace objs {

        template<int addr>
        class memory : public evaluable{
        private:
            //inputs
            bit* addr_i[addr];
            bit* val_i[8];
            bit* rw;
            bit* CE;
            bit* CLK;
            bit* RST;

            //outputs
            bit* val_o[8];
            bit* ready;

            //internal variables
            enum st{LISTENING, DONE};
            st state = LISTENING;
            u_int8_t mem[1<<addr];
        public:
            memory(bit**, bit**, bit*, bit*, bit*, bit*, bit**, bit *&);
            ~memory();
            void eval() override;
            void flag_for_eval(triggering*) override;
        };

        template<int addr>
        void memory<addr>::flag_for_eval(triggering* to_flag) {
            if(to_flag == RST || flag == nullptr)
                flag = to_flag;
        }

        template<int addr>
        void memory<addr>::eval() {
            if(flag == RST || RST->get_content()){
                for(int i = 0; i < 1 << addr; i++) {
                    mem[i] = 0;
                }
            } else if(flag == CLK) {
                if(!CE->get_content()){
                    ready->set_content(false);
                    for(int i = 0; i < 8; i++){
                        val_o[i]->set_content(false);
                    }
                    return;
                }

                switch(state){
                case LISTENING: {
                    ready->set_content(false);
                    u_int32_t input_addr = 0;
                    for (int i = 0; i < addr; i++) {
                        input_addr *= 2;
                        input_addr &= addr_i[i]->get_content();
                    }
                    if (rw->get_content()) { //write
                        u_int8_t val_in = 0;
                        for (int i = 0; i < 8; i++) {
                            val_in *= 2;
                            val_in &= val_i[7-i]->get_content();
                        }
                        mem[input_addr] = val_in;
                    } else { //read
                        for (int i = 0; i < 8; i++) {
                            val_o[i]->set_content((mem[input_addr] >> i) & 1);
                        }
                    }
                    state = DONE;
                }
                    break;
                case DONE: {
                    ready->set_content(true);
                    state = LISTENING;
                }
                }
            }
        }

        template<int addr>
        memory<addr>::memory(bit * addr_i[], bit * val_i[], bit *rw, bit *CE, bit *CLK, bit* RST, bit * val_o[], bit*& ready) {
            for(int i = 0; i < addr; i++)
                this->addr_i[i] = addr_i[i];
            for(int i = 0; i < 8; i++)
                this->val_i[i] = val_i[i];
            this->rw = rw;
            this->CE = CE;
            this->CLK = CLK;
            this->RST = RST;
            int new_expected_level = 0;
            for(int i = 0; i < addr; i++)
                new_expected_level = std::max(new_expected_level, addr_i[i]->get_expected_level());
            for(int i = 0; i < 8; i++)
                new_expected_level = std::max(new_expected_level, val_i[i]->get_expected_level());
            new_expected_level = std::max(new_expected_level, rw->get_expected_level());
            new_expected_level = std::max(new_expected_level, CE->get_expected_level());
            new_expected_level = std::max(new_expected_level, CLK->get_expected_level());
            new_expected_level = std::max(new_expected_level, RST->get_expected_level());

            set_expected_level(new_expected_level + 1);

            for(int i = 0; i < 8; i++){
                this->val_o[i] = new bit(true);
                this->val_o[i]->set_expected_level(get_expected_level());
                val_o[i] = this->val_o[i];
            }
            this->ready = new bit(false);
            this->ready->set_expected_level(get_expected_level());
            ready = this->ready;

            CLK->add_trigger(this,triggering::NEGATIVE);
            RST->add_trigger(this,triggering::POSITIVE);
        }

        template<int addr>
        memory<addr>::~memory() {
            for(int i = 0; i < 8; i++){
                delete val_o[i];
            }
            delete ready;
        }



    } // sim
} // objs

#endif //SIMULATOR_MEMORY_H
