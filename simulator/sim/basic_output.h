//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_BASIC_OUTPUT_H
#define SIMULATOR_BASIC_OUTPUT_H

#include "basic_io.h"
#include "bit.h"

namespace sim {

    class basic_output : public basic_io{
    public:
        objs::bit& input;
        bool render_value = false;
        explicit basic_output(objs::bit &input);
        void update() override;
        virtual void render() const;
    };

} // sim

#endif //SIMULATOR_BASIC_OUTPUT_H
