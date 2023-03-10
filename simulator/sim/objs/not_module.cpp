//
// Created by mihai-laptop on 11/26/22.
//

#include "not_module.h"

namespace sim {
    namespace objs {
        not_module::not_module(bit* in, bit*& out) {
            this->in = in;
            this->out = new bit(!in->get_content());
            this->set_expected_level(in->get_expected_level()+1);
            this->out->set_expected_level(this->get_expected_level());
            out = this->out;
            in->add_trigger(this,triggering::BOTH);
        }

        not_module::~not_module() {
            delete this->out;
        }

        void not_module::eval() {
            this->flag = nullptr;
            this->out->set_content(!in->get_content());
        }
    } // sim
} // objs