//
// Created by mihai-laptop on 3/28/23.
//

#include <cassert>
#include "word.h"

namespace sim {
    namespace objs {
        word::word() = default;

        word::word(const word &other) {
            for(int i = 0; i < 16; i++){
                c[i] = other.c[i];
            }
        }

        bit &word::operator[](int i) const {
            assert(i >= 0 && i < 16);
            return *c[i];
        }

        void word::connect(bit &b, int i) {
            assert(i >= 0 && i < 16);
            c[i] = &b;
        }
    } // sim
} // objs