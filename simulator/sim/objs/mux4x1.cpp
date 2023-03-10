//
// Created by Mihai - PC on 11/24/2022.
//

#include "mux4x1.h"

namespace sim {
    namespace objs {
        mux4x1::mux4x1(bit* a, bit* b, bit* c, bit* d, bit* sel0, bit* sel1, bit*& out) {
            this->a = a;
            this->b = b;
            this->c = c;
            this->d = d;
            this->sel0 = sel0;
            this->sel1 = sel1;
            this->out = new bit((a->get_content() && (!sel0->get_content() && !sel1->get_content())) ||
                                (b->get_content() && ( sel0->get_content() && !sel1->get_content())) ||
                                (c->get_content() && (!sel0->get_content() &&  sel1->get_content())) ||
                                (d->get_content() && ( sel0->get_content() &&  sel1->get_content())));
            int max_level = 0;
            max_level = std::max(a   ->get_expected_level(),max_level);
            max_level = std::max(b   ->get_expected_level(),max_level);
            max_level = std::max(c   ->get_expected_level(),max_level);
            max_level = std::max(d   ->get_expected_level(),max_level);
            max_level = std::max(sel0->get_expected_level(),max_level);
            max_level = std::max(sel1->get_expected_level(),max_level);
            this->out->set_expected_level(max_level + 1);
            this->set_expected_level(max_level + 1);
            out = this->out;

            a   ->add_trigger(this,triggering::BOTH);
            b   ->add_trigger(this,triggering::BOTH);
            c   ->add_trigger(this,triggering::BOTH);
            d   ->add_trigger(this,triggering::BOTH);
            sel0->add_trigger(this,triggering::BOTH);
            sel1->add_trigger(this,triggering::BOTH);
        }

        mux4x1::~mux4x1() {
            delete this->out;
        }

        void mux4x1::eval() {
            this->flag = nullptr;
            this->out->set_content( (a->get_content() && (!sel0->get_content() && !sel1->get_content())) ||
                                    (b->get_content() && ( sel0->get_content() && !sel1->get_content())) ||
                                    (c->get_content() && (!sel0->get_content() &&  sel1->get_content())) ||
                                    (d->get_content() && ( sel0->get_content() &&  sel1->get_content())));
        }
    } // sim
} // objs