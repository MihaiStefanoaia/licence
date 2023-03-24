//
// Created by mihai-laptop on 3/13/23.
//

#ifndef SIMULATOR_MEMORY_TEST_H
#define SIMULATOR_MEMORY_TEST_H


#include "bit.h"
#include "memory.h"
#include "environment.h"
#include <sys/types.h>

namespace sim {
    namespace test {

        using sim::objs::bit;
        using sim::objs::memory;
        class memory_test : public sim::environment {
            bit* CLK;
            bit* CE;
            bit* addr_i[16];
            bit* val_i[8];
            bit* val_o[8];
            bit* ready;
            bit* rw;
            bit* RST;
            memory<16>* mem;
        public:
            memory_test();
            ~memory_test();
            void write(u_int16_t addr, u_int8_t val);
            u_int8_t read(u_int16_t addr);
            void reset();

            void step() override;
        };

    } // sim
} // test

#endif //SIMULATOR_MEMORY_TEST_H
