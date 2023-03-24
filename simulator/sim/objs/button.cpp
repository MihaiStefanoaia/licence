//
// Created by mihai-laptop on 3/22/23.
//

#include "button.h"

namespace sim {
    namespace objs {
        button::button(bit *& output) {
            this->output = new bit();
            output = this->output;
        }

        button::~button() {
            delete output;
        }

        void button::press() {
            output->set_content(true);
        }

        void button::release() {
            output->set_content(false);
        }
    } // sim
} // objs