//
// Created by Mihai - PC on 11/16/2022.
//

#ifndef SIMULATOR_MODULE_H
#define SIMULATOR_MODULE_H

#include "../evaluable.h"
#include "../evaluation_list.h"

namespace sim {
    namespace objs {

        class module : public evaluable {
        protected:
            evaluation_list* internal;
            module();
            ~module();
        public:
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_MODULE_H
