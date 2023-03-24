//
// Created by mihai-laptop on 3/22/23.
//

#include "byte.h"

namespace sim {
    namespace objs {
        void eval(){

        }

        byte::byte(bit **in, bit **out) {
            int exl = 0;
            for(int i = 0; i < 8; i++){
                this->in[i] = in[i];
                if(in[i] != nullptr){
                    exl = std::max(exl,in[i]->get_expected_level());
                }
            }
            exl++;
            set_expected_level(exl);
            for(int i = 0; i < 8; i++){
                this->out[i] = new bit();
                this->out[i]->set_expected_level(exl+1);
                out[i] = this->out[i];
            }
        }
    } // sim
} // objs