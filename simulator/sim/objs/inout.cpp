//
// Created by Mihai - PC on 11/16/2022.
//

#include "inout.h"

namespace sim {
    namespace objs {
        void inout::eval() {
            this->reg->set_content(in_a->get_content() || in_b->get_content());
        }

        ///input a non initialized wire* into the out field
        ///it will generate a new object which will be used as the output
        ///do not delete the output pointer outside this class
        inout::inout(wire* in_a, wire* in_b, wire*& out) {
            this->in_a = in_a;
            this->in_b = in_b;
            this->reg = new bit(in_a->get_content() || in_b->get_content());
            this->reg->set_expected_level(std::max(in_a->get_expected_level(),in_b->get_expected_level())+1);
            this->set_expected_level(this->reg->get_expected_level());
            this->out  = new wire(this->reg);
            out = this->out;
        }

        inout::~inout() {
            delete this->reg;
            delete this->out;
        }
    } // sim
} // objs