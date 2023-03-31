//
// Created by mihai-laptop on 3/28/23.
//

#include "byte.h"
#include <cassert>
#include <iostream>

namespace sim {
    namespace objs {
        byte::byte() = default;
        byte::byte(const byte& other){
            for(auto i = 0; i < 8; i++){
                c[i] = other.c[i];
            }
        }

        bit &byte::operator[](int i) const {
#ifndef SAFETIES_OFF
            assert(i >= 0 && i < 8);
#endif
            return *c[i];
        }

        void byte::connect(bit &b, int i) {
#ifndef SAFETIES_OFF
            assert(c[i] == nullptr && i >= 0 && i < 8);
#endif
            c[i] = &b;
        }
    } // sim
} // objs