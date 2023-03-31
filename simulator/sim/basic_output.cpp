//
// Created by mihai-pc on 4/1/23.
//

#include <iostream>
#include "basic_output.h"

namespace sim {
    basic_output::basic_output(objs::bit &input) : input(input) {}

    void basic_output::update() {
        render_value = input.get_content();
    }

    void basic_output::render() const {
        std::cout << (render_value ? "ON" : "OFF") << '\n';
    }
} // sim