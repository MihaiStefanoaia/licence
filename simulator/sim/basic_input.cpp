//
// Created by mihai-pc on 4/1/23.
//

#include "basic_input.h"

namespace sim {
    void basic_input::update() {
        output.set_content(buffer);
        to_update = false;
    }

    basic_input::basic_input(objs::bit &output) : output(output) {}

    void basic_input::set_value(bool value) {
        buffer = value;
        to_update = true;
    }
} // sim