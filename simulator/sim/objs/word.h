//
// Created by mihai-laptop on 3/28/23.
//

#ifndef SIMULATOR_WORD_H
#define SIMULATOR_WORD_H

#include "bit.h"

namespace sim {
    namespace objs {

        class word {
            bit* c[16] = {nullptr};
        public:
            word();
            word(const word&);
            void connect(bit&,int);
            bit& operator[](int) const;
        };

    } // sim
} // objs

#endif //SIMULATOR_WORD_H
