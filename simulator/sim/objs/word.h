//
// Created by mihai-laptop on 3/28/23.
//

#ifndef SIMULATOR_WORD_H
#define SIMULATOR_WORD_H

#include "bit.h"
#include "bit_array.h"

namespace sim {
    namespace objs {

        class word : public bit_array {
        public:
            word();
            word(const word&);
        };

    } // sim
} // objs

#endif //SIMULATOR_WORD_H
