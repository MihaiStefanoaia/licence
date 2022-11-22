//
// Created by Mihai - PC on 11/16/2022.
//

#include "module.h"

namespace sim {
    namespace objs {
        module::module():evaluable(){
            internal = new evaluation_list();
        }
        module::~module() {
            delete internal;
        }
        void module::eval() {
            this->internal->eval();
        }
    } // sim
} // objs