//
// Created by mihai-laptop on 3/28/23.
//

#include <cassert>
#include "word.h"
#include <iostream>

namespace sim {
    namespace objs {
        word::word() = default;

        word::word(const word &other) {
            for(int i = 0; i < 16; i++){
                c[i] = other.c[i];
            }
        }

        bit &word::operator[](int i) const {
#ifndef SAFETIES_OFF
            assert(i >= 0 && i < 16);
#endif
            return *c[i];
        }

        void word::connect(bit &b, int i) {
#ifndef SAFETIES_OFF
            assert(i >= 0 && i < 16);
#endif
            c[i] = &b;
        }
    } // sim
} // objs