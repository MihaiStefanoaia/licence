//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_MUX2X1_H
#define SIMULATOR_MUX2X1_H

#include "module.h"
#include "bit.h"

namespace sim {
    namespace objs {

        class mux2x1 : module{
        private:
            bit* a;
            bit* b;
            bit* sel;
            bit* out;
        public:
            mux2x1(bit*,bit*,bit*,bit*&);
            ~mux2x1();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_MUX2X1_H
