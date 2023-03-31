//
// Created by mihai-laptop on 3/22/23.
//

#ifndef SIMULATOR_BUTTON_H
#define SIMULATOR_BUTTON_H

#include "bit.h"
#include "basic_input.h"

namespace sim {
    namespace objs {

        class button : public basic_input{
        public:
            explicit button(bit& a) : basic_input(a){};
            void press() { set_value(true); };
            void release() { set_value(false); };
        };

    } // sim
} // objs

#endif //SIMULATOR_BUTTON_H
