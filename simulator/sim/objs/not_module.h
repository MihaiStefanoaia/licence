//
// Created by mihai-laptop on 11/26/22.
//

#ifndef SIMULATOR_NOT_MODULE_H
#define SIMULATOR_NOT_MODULE_H

#include "bit.h"

namespace sim {
    namespace objs {

        class not_module : public evaluable{
        private:
            bit& in;
            bit& out;
        public:
            not_module(bit&,bit&);
            ~not_module() override;
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_NOT_MODULE_H
