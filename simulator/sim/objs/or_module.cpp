//
// Created by mihai-laptop on 11/26/22.
//

//
// Created by Mihai - PC on 11/22/2022.
//

#include "or_module.h"

namespace sim {
    namespace objs {

        ///input a non initialized bit* into the out field
        ///it will generate a new object which will be used as the output
        ///do not delete the output pointer outside this class
        or_module::or_module(bit* a, bit* b, bit*& out) {
            this->a = a;
            this->b = b;
            this->out = new bit(a->get_content() && b->get_content());
            this->out->set_expected_level(std::max(a->get_expected_level(),b->get_expected_level())+1);
            this->set_expected_level(this->out->get_expected_level());
            out = this->out;
            a->add_trigger(this,sim::triggering::BOTH);
            b->add_trigger(this,sim::triggering::BOTH);
        }

        void or_module::eval() {
            this->flag = nullptr;
            this->out->set_content(this->a->get_content() || this->b->get_content());
        }

        or_module::~or_module() {
            delete this->out;
        }
    } // sim
} // objs