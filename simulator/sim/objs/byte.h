//
// Created by mihai-laptop on 3/28/23.
//

#ifndef SIMULATOR_BYTE_H
#define SIMULATOR_BYTE_H

#include "bit.h"
#include "bit_array.h"

namespace sim {
    namespace objs {

        class byte : public bit_array{
        public:
            byte();
            byte(const byte&);
        };

    } // sim
} // objs

#endif //SIMULATOR_BYTE_H
