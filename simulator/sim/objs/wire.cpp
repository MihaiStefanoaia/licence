//
// Created by Mihai - PC on 11/16/2022.
//

#include "wire.h"
#include <stdexcept>
#include <iostream>

namespace sim {
    namespace objs {
        wire::wire(bit* b) {

            this->source = b;
            if(b != nullptr){
                this->content = b->get_content();
                b->add_trigger(this,BOTH);
                this->expected_level = b->get_expected_level() + 1;
                std::cout<<"initialized wire with level "<< this->get_expected_level()<<'\n';
            }
        }

        void wire::eval() {
            this->flag = false;
            if(this->content != this->source->get_content()){
                if(this->content){
                    this->on_value_change(NEGATIVE);
                } else {
                    this->on_value_change(POSITIVE);
                }
            }
            this->content = this->source->get_content();
        }
    } // sim
} // objs