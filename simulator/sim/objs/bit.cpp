//
// Created by Mihai - PC on 11/15/2022.
//

#include "bit.h"

namespace sim {
    namespace objs {
        bit::bit(bool init) {
            this->content = init;
        }

        bool bit::get_content() const {
            return this->content;
        }

        void bit::set_content(bool new_content) {
            if(this->content != new_content){
                if(new_content){
                    this->on_value_change(POSITIVE);
                } else {
                    this->on_value_change(NEGATIVE);
                }
                this->content = new_content;
            }
        }
    } // sim
} // objs