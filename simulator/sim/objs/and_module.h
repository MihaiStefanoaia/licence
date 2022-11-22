//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_AND_MODULE_H
#define SIMULATOR_AND_MODULE_H

#include "bit.h"
#include "module.h"
#include "wire.h"

namespace sim {
    namespace objs {
        class and_module : public module{
        private:
            wire* a;
            wire* b;
            bit* reg;
            wire* out;
        public:
            and_module(wire*, wire*, wire*&);
            ~and_module();
            void eval() override;
        };
    } // sim
} // objs

#endif //SIMULATOR_AND_MODULE_H
