//
// Created by mihai-laptop on 3/13/23.
//

#ifndef SIMULATOR_ENVIRONMENT_H
#define SIMULATOR_ENVIRONMENT_H

#include "evaluation_list.h"
#include <map>
#include "objs/bit.h"

namespace sim {

    class environment {
    protected:
        std::map<std::string,objs::bit*> wire_db;
        std::map<std::string,evaluable*> module_db;
        evaluation_list evl;

        virtual void step(){}
    };

} // sim

#endif //SIMULATOR_ENVIRONMENT_H
