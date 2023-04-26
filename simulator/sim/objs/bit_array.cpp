//
// Created by mihai-laptop on 11/25/22.
//

#include "bit_array.h"

namespace sim {
    namespace objs {
        void bit_array::connect(bit &b, int i) {
#ifndef SAFETIES_OFF
            assert(content[i] == nullptr && i >= 0 && i < size);
#endif
            content[i] = &b;
        }

        bit& bit_array::operator[](int i) const {
#ifndef SAFETIES_OFF
            assert(i >= 0 && i < size);
#endif
            return *content[i];
        }

        bit_array::bit_array(int size){
            this->size = size;
            content = new bit*[size];
        }

        int bit_array::get_size() const {
            return size;
        }

    } // sim
} // objs