//
// Created by Mihai - PC on 11/22/2022.
//

#include "and_module.h"
#include <cassert>

namespace sim {
    namespace objs {

        ///input a non initialized bit* into the out field
        ///it will generate a new object which will be used as the output
        ///do not delete the output pointer outside this class
        and_module::and_module(bit& a, bit& b, bit& out) : a(a),b(b),out(out){
            assert(out.get_expected_level() == 0);
            out.set_expected_level(std::max(a.get_expected_level(),b.get_expected_level())+1);
            set_expected_level(out.get_expected_level());
            a.add_trigger(this,sim::triggering::BOTH);
            b.add_trigger(this,sim::triggering::BOTH);
        }

        void and_module::eval() {
            flag = nullptr;
            out.set_content(a.get_content() && b.get_content());
        }

        and_module::~and_module() = default;
    } // sim
} // objs