//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_BASIC_INPUT_H
#define SIMULATOR_BASIC_INPUT_H

#include "basic_io.h"
#include "bit.h"

namespace sim {

    class basic_input : public basic_io{
        objs::bit& output;
        bool buffer = false;
        bool to_update = false;
    public:
        explicit basic_input(objs::bit &output);
        virtual ~basic_input() = default;
        void update() override;
        void set_value(bool);
        friend class environment;
    };

} // sim

#endif //SIMULATOR_BASIC_INPUT_H
