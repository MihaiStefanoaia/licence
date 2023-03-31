//
// Created by mihai-laptop on 11/26/22.
//

#include "not_module.h"

namespace sim {
    namespace objs {
        not_module::not_module(bit& in, bit& out) : in(in), out(out) {
            set_expected_level(in.get_expected_level()+1);
            out.set_expected_level(get_expected_level());
            in.add_trigger(this,triggering::BOTH);
        }

        not_module::~not_module() = default;

        void not_module::eval() {
            flag = nullptr;
            out.set_content(!in.get_content());
        }
    } // sim
} // objs