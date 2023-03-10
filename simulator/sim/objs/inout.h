//
// Created by Mihai - PC on 11/16/2022.
//

#ifndef SIMULATOR_INOUT_H
#define SIMULATOR_INOUT_H

#include "bit.h"

namespace sim {
    namespace objs {

        class inout : public evaluable{
        private:
            bit* in_a;
            bit* in_b;
            bit* out;
        public:
            inout(bit*,bit*,bit*&);
            ~inout();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_INOUT_H
