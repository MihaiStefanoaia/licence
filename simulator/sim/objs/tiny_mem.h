//
// Created by mihai-laptop on 5/15/23.
//

#ifndef SIMULATOR_TINY_MEM_H
#define SIMULATOR_TINY_MEM_H

#include "bit_array.h"
#include "basic_io.h"

namespace sim {
    namespace objs {

        class tiny_mem : public evaluable {
            bit_array addr_i;
            bit_array data_i;
            bit_array data_o;
            bit& active;
            bit& rw;
            bit& ready;
            bit& CLK;
            bit& RST;
            enum states {IDLE, READ_START, WAIT, READ_FINISH, WRITE_START, WRITE_FINISH};
            states state = IDLE;
            states next_state = IDLE;
            u_int8_t data[16] = {0};
            u_int8_t data_buf = 0;
            u_int8_t addr_buf = 0;
        public:
            tiny_mem(const bit_array&, const bit_array&, const bit_array&, bit&, bit&, bit&, bit&,bit&);
            void eval() override;
            void flag_for_eval(sim::triggering *ev) override;
        };

    } // sim
} // objs

#endif //SIMULATOR_TINY_MEM_H
