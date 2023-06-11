//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_AND_MODULE_H
#define SIMULATOR_AND_MODULE_H

#include "bit.h"

#define AND_MODULE_CLASS_NAME "and_module"
#define AND_MODULE_ARG_DECL {"and_module", {1,1,1}}
#define AND_MODULE_ARG_READ {"and_module", {true,true,false}}


namespace sim {
    namespace objs {
        class and_module : public evaluable{
        private:
            bit& a;
            bit& b;
            bit& out;
        public:
            and_module(bit&, bit&, bit&, bool = true);
            ~and_module() override;
            void eval() override;
            static void init_class();
        };
    } // sim
} // objs

#endif //SIMULATOR_AND_MODULE_H
