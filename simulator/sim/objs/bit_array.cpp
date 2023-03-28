//
// Created by mihai-laptop on 11/25/22.
//

#include "bit_array.h"

namespace sim {
    namespace objs {
        void bit_array::connect(bit &b, int i) {
            assert(content[i] == nullptr && i >= 0 && i < size);
            content[i] = &b;
        }

        bit& bit_array::operator[](int i) const {
            assert(i >= 0 && i < size);
            return *content[i];
        }

        bit_array::bit_array(int size){
            this->size = size;
            content = new bit*[size];
        }

    } // sim
} // objs