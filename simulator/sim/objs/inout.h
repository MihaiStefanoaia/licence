//
// Created by Mihai - PC on 11/16/2022.
//

#ifndef SIMULATOR_INOUT_H
#define SIMULATOR_INOUT_H

#include "module.h"
#include "wire.h"

namespace sim {
    namespace objs {

        class inout : public module{
        private:
            wire* in_a;
            wire* in_b;
            wire* out;
            bit* reg;
        public:
            inout(wire*,wire*,wire*&);
            ~inout();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_INOUT_H
