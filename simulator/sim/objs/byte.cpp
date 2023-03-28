//
// Created by mihai-laptop on 3/28/23.
//

#include "byte.h"
#include <cassert>

namespace sim {
    namespace objs {
        byte::byte() = default;
        byte::byte(const byte& other){
            for(auto i = 0; i < 8; i++){
                c[i] = other.c[i];
            }
        }

        bit &byte::operator[](int i) const {
            assert(i >= 0 && i < 8);
            return *c[i];
        }

        void byte::connect(bit &b, int i) {
            assert(c[i] == nullptr && i >= 0 && i < 8);
            c[i] = &b;
        }
    } // sim
} // objs