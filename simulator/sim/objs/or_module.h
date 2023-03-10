//
// Created by mihai-laptop on 11/26/22.
//

#ifndef SIMULATOR_OR_MODULE_H
#define SIMULATOR_OR_MODULE_H

#include "bit.h"

namespace sim {
    namespace objs {

        class or_module : public evaluable{
        private:
            bit* a;
            bit* b;
            bit* out;
        public:
            or_module(bit*, bit*, bit*&);
            ~or_module();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_OR_MODULE_H
