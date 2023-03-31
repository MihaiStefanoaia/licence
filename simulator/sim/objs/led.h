//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_LED_H
#define SIMULATOR_LED_H

#include "basic_output.h"

namespace sim {
    namespace objs {

        class led : public basic_output{
        public:
            explicit led(bit& a) : basic_output(a){};
        };

    } // sim
} // objs

#endif //SIMULATOR_LED_H
