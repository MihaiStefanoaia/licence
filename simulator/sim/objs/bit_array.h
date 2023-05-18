//
// Created by mihai-laptop on 11/25/22.
//

#ifndef SIMULATOR_BIT_ARRAY_H
#define SIMULATOR_BIT_ARRAY_H

#include <cstdarg>
#include <stdexcept>
#include <cassert>
#include "bit.h"

namespace sim {
    namespace objs {
        class bit_array {
        protected:
            bit** content;
            int size;
        public:
            explicit bit_array(int = 8);
            bit_array(const bit_array&);
            ~bit_array();
            void connect(bit&, int);
            bit& operator [](int) const;
            int get_size() const;
        };

    } // sim
} // objs

#endif //SIMULATOR_BIT_ARRAY_H
