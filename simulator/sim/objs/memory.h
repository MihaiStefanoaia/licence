//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "bit.h"
#include "module.h"
#include <sys/types.h>

namespace sim {
    namespace objs {

        template<int addr>
        class memory : protected module{
        private:
            //inputs
            bit* addr_i[addr];
            bit* val_i[8];
            bit* rw;
            bit* CE;
            bit* CLK;

            //outputs
            bit* val_o[8];
            bit* ready;

            //internal variables
            u_int8_t mem[1<<addr];
        };

    } // sim
} // objs

#endif //SIMULATOR_MEMORY_H
