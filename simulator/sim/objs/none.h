//
// Created by mihai-laptop on 5/8/23.
//

#ifndef SIMULATOR_NONE_H
#define SIMULATOR_NONE_H

#include "bit.h"

namespace sim {
    namespace objs {

        class none : public bit{
        public:
            none() : bit(false) {};
            void set_content(bool) override {};
        };

    } // sim
} // objs

#endif //SIMULATOR_NONE_H
