//
// Created by Mihai - PC on 11/22/2022.
//

#include "and_module.h"

namespace sim {
    namespace objs {

        ///input a non initialized wire* into the out field
        ///it will generate a new object which will be used as the output
        ///do not delete the output pointer outside this class
        and_module::and_module(wire* a, wire* b, wire*& out) {
            this->a = a;
            this->b = b;
            this->reg = new bit(a->get_content() && b->get_content());
            this->reg->set_expected_level(std::max(a->get_expected_level(),b->get_expected_level())+1);
            this->set_expected_level(this->reg->get_expected_level());
            this->out = new wire(this->reg);
            out = this->out;
            a->add_trigger(this,sim::triggering::BOTH);
            b->add_trigger(this,sim::triggering::BOTH);
        }

        void and_module::eval() {
            this->flag = false;
            this->reg->set_content(this->a->get_content() && this->b->get_content());
        }

        and_module::~and_module() {
            delete this->reg;
            delete this->out;
        }
    } // sim
} // objs