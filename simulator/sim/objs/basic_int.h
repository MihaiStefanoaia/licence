//
// Created by Mihai - PC on 11/15/2022.
//

#ifndef SIMULATOR_BASIC_INT_H
#define SIMULATOR_BASIC_INT_H

#include "../triggering.h"
#include <sys/types.h>
namespace sim {
    namespace objs {

        class basic_int : public sim::triggering {
        private:

            int content = 0;
        public:
            basic_int() : sim::triggering() {};
        };

    } // objs
}

#endif //SIMULATOR_BASIC_INT_H
