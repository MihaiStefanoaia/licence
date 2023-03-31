//
// Created by mihai-laptop on 3/22/23.
//

#ifndef SIMULATOR_BUTTON_H
#define SIMULATOR_BUTTON_H

#include "bit.h"

namespace sim {
    namespace objs {

        class button{
            bit& output;
            button(bit&);
            void press();
            void release();
        };

    } // sim
} // objs

#endif //SIMULATOR_BUTTON_H
