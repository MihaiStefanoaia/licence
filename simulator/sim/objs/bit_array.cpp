//
// Created by mihai-laptop on 11/25/22.
//

#include "bit_array.h"

namespace sim {
    namespace objs {
        void bit_array::connect(bit &b, int i) {
            if(!(i >= 0 && i < size))
                throw std::runtime_error("array access out of bounds");
            content[i] = &b;
        }

        bit& bit_array::operator[](int i) const {
            if(!(i >= 0 && i < size))
                throw std::runtime_error("array access out of bounds");
            return *content[i];
        }

        bit_array::bit_array(int size){
            this->size = size;
            content = new bit*[size];
        }

        int bit_array::get_size() const {
            return size;
        }

        bit_array::bit_array(const bit_array &other){
            size = other.size;
            content = new bit*[size];
            for(int i = 0; i < size; i++)
                content[i] = other.content[i];
        }

        bit_array::~bit_array() {
            delete content;
        };
    } // sim
} // objs