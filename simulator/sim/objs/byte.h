//
// Created by mihai-laptop on 3/28/23.
//

#ifndef SIMULATOR_BYTE_H
#define SIMULATOR_BYTE_H

#include "bit.h"

namespace sim {
    namespace objs {

        class byte {
            bit* c[8] = {nullptr};
        public:
            byte();
            byte(const byte&);
            void connect(bit&,int);
            bit& operator[](int) const;
        };

    } // sim
} // objs

#endif //SIMULATOR_BYTE_H
