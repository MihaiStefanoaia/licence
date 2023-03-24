//
// Created by mihai-laptop on 3/13/23.
//

#ifndef SIMULATOR_ENVIRONMENT_H
#define SIMULATOR_ENVIRONMENT_H

#include "evaluation_list.h"

namespace sim {

    class environment {
    protected:
        evaluation_list evl;
        virtual void step(){}
    };

} // sim

#endif //SIMULATOR_ENVIRONMENT_H
