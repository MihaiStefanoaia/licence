//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_AND_MODULE_H
#define SIMULATOR_AND_MODULE_H

#include "bit.h"

namespace sim {
    namespace objs {
        class and_module : public evaluable{
        private:
            bit* a;
            bit* b;
            bit* out;
        public:
            and_module(bit*, bit*, bit*&);
            ~and_module();
            void eval() override;
        };
    } // sim
} // objs

#endif //SIMULATOR_AND_MODULE_H
