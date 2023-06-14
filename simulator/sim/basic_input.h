//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_BASIC_INPUT_H
#define SIMULATOR_BASIC_INPUT_H

#include "basic_io.h"
#include "bit.h"

namespace sim {

    class basic_input : public basic_io{
    public:
        basic_input() = default;
        ~basic_input() override = default;
        void update() override = 0;
        friend class environment;
    };

} // sim

#endif //SIMULATOR_BASIC_INPUT_H
