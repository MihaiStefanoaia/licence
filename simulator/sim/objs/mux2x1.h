//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_MUX2X1_H
#define SIMULATOR_MUX2X1_H

#include "wire.h"
#include "module.h"

namespace sim {
    namespace objs {

        class mux2x1 : module{
        private:
            wire* a;
            wire* b;
            wire* sel;
            bit* reg;
            wire* out;
        public:
            mux2x1(wire*,wire*,wire*,wire**);
            ~mux2x1();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_MUX2X1_H
