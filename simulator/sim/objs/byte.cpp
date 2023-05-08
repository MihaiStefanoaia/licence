//
// Created by mihai-laptop on 3/28/23.
//

#include "byte.h"
#include <cassert>
#include <iostream>

namespace sim {
    namespace objs {
        byte::byte() : bit_array(8) {}
        byte::byte(const byte& other) : bit_array(other) {
        }
    } // sim
} // objs