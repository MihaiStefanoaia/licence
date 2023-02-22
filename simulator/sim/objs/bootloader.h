//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_BOOTLOADER_H
#define SIMULATOR_BOOTLOADER_H

#include "module.h"
#include "bit.h"

namespace sim {
    namespace objs {

        class bootloader : protected module{
            //inputs
            bit* start;
            bit* CLK;

            //outputs
            bit* mem_en;
            bit* addr[16];
            bit* value[8];
            bit* mem_rw;

            //internal variables
            std::string path;
        };

    } // sim
} // objs

#endif //SIMULATOR_BOOTLOADER_H
