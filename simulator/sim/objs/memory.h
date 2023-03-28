//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "bit.h"
#include "byte.h"
#include "word.h"
#include <sys/types.h>

namespace sim {
    namespace objs {

        class memory : public evaluable{
        private:
            //inputs
            word& addr_i;
            byte& val_i;
            bit& rw;
            bit& CE;
            bit& CLK;
            bit& RST;

            //outputs
            byte& val_o;
            bit& ready;

            //internal variables
            enum st{LISTENING, DONE};
            st state = LISTENING;
            u_int8_t mem[1<<16] = {0};
        public:
            memory(word&, byte&, bit&, bit&, bit&, bit&, byte&, bit &);
            void eval() override;
            void flag_for_eval(triggering*) override;
        };

    } // sim
} // objs

#endif //SIMULATOR_MEMORY_H
