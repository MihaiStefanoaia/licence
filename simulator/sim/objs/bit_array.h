//
// Created by mihai-laptop on 11/25/22.
//

#ifndef SIMULATOR_BIT_ARRAY_H
#define SIMULATOR_BIT_ARRAY_H

#include <cstdarg>
#include <stdexcept>
#include "bit.h"

namespace sim {
    namespace objs {
        template<int size>
        class bit_array {
        private:
            bit* content[size];
        public:
            bit_array(std::initializer_list<bit*>);
            bit* operator [](int);
        };

        template<int size>
        bit *bit_array<size>::operator[](int i) {
            return content[i];
        }

        template<int size>
        bit_array<size>::bit_array(const std::initializer_list<bit *> init) {
            if(init.size() != size){
                throw std::invalid_argument("Size mismatch between initializer list and template declaration");
            }
            int i = 0;
            for(auto it = init.begin(); i < size; i++, ++it){
                content[i] = *it;
            }
        }

    } // sim
} // objs

#endif //SIMULATOR_BIT_ARRAY_H
