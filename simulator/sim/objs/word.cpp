//
// Created by mihai-laptop on 3/28/23.
//

#include <cassert>
#include "word.h"
#include <iostream>

namespace sim {
    namespace objs {
        word::word() = default;

        word::word(const word &other) : bit_array(other) {}
    } // sim
} // objs