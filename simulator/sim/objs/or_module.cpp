//
// Created by mihai-laptop on 11/26/22.
//

//
// Created by Mihai - PC on 11/22/2022.
//

#include "or_module.h"

namespace sim {
    namespace objs {
        or_module::or_module(bit& a, bit& b, bit& out) : a(a), b(b), out(out) {
            set_expected_level(std::max(a.get_expected_level(),b.get_expected_level())+1);
            out.set_expected_level(get_expected_level());
            a.add_trigger(this,sim::triggering::BOTH);
            b.add_trigger(this,sim::triggering::BOTH);
        }

        void or_module::eval() {
            flag = nullptr;
            out.set_content(a.get_content() || b.get_content());
        }

        or_module::~or_module() = default;
    } // sim
} // objs