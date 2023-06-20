//
// Created by mihai-laptop on 5/9/23.
//

#ifndef SIMULATOR_TINY_CPU_H
#define SIMULATOR_TINY_CPU_H

#include "evaluable.h"
#include "bit_array.h"
#include <sys/types.h>

#define ARGS_SIZES_TINY_CPU {"tiny_cpu",{4,4,4,1,1,1,4,4,1,1,1}

namespace sim {
    namespace objs {

        class tiny_cpu : public evaluable{
            //memory interface
            bit_array addr_o;
            bit_array data_i;
            bit_array data_o;
            bit& active;
            bit& rw;
            bit& ready;

            //the port
            bit_array port_i;
            bit_array port_o;

            //bits
            bit& interr;
            bit& CLK;
            bit& RST;

            u_int8_t reg = 0;
            bool rw_mode = false;


            enum states {POLL, READ_MEM, WRITE_MEM, WAIT_FOR_MEMORY, WRITE_PORT};

            states state = POLL;
            states next_state = POLL;

        public:
            tiny_cpu(const bit_array&,const bit_array&,const bit_array&,bit&,bit&,bit&,const bit_array&,const bit_array&,bit&,bit&,bit&);
            void eval() override;
            void flag_for_eval(sim::triggering *ev) override;
        };

    } // sim
} // objs

#endif //SIMULATOR_TINY_CPU_H
