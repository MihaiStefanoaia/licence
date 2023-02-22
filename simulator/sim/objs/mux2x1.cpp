//
// Created by Mihai - PC on 11/22/2022.
//

#include "mux2x1.h"

namespace sim {
    namespace objs {
        mux2x1::mux2x1(bit* a, bit* b, bit* sel, bit*& out) {
            this->a = a;
            this->b = b;
            this->sel = sel;
            this->set_expected_level(std::max(std::max(a->get_expected_level(),b->get_expected_level()),sel->get_expected_level())+1);
            this->out = new bit((a->get_content() && !sel->get_content()) || (b->get_content() && sel->get_content()));
            this->out->set_expected_level(this->get_expected_level());
            out = this->out;
            a->add_trigger(this,triggering::BOTH);
            b->add_trigger(this,triggering::BOTH);
            sel->add_trigger(this,triggering::BOTH);
        }

        mux2x1::~mux2x1() {
            delete this->out;
        }

        void mux2x1::eval() {
            this->flag = false;
            this->out->set_content((a->get_content() && !sel->get_content()) || (b->get_content() && sel->get_content()));
        }
    } // sim
} // objs