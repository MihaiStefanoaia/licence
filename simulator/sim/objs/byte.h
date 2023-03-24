//
// Created by mihai-laptop on 3/22/23.
//

#ifndef SIMULATOR_BYTE_H
#define SIMULATOR_BYTE_H

#include <sys/types.h>
#include "evaluable.h"
#include "bit.h"

namespace sim {
    namespace objs {

        class byte : public evaluable {
            bit* in[8] = {nullptr};
            bit* out[8] = {nullptr};
            u_int8_t content = 0;

            byte(bit*[], bit **);
            virtual void eval();
        };

    } // sim
} // objs

#endif //SIMULATOR_BYTE_H
