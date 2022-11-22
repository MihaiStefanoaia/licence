//
// Created by Mihai - PC on 11/16/2022.
//

#ifndef SIMULATOR_WIRE_H
#define SIMULATOR_WIRE_H

#include "bit.h"
#include "bit_array.h"
#include "../triggering.h"

namespace sim {
    namespace objs {

        class wire : public triggering{
        private:
            bool content;
            bit* source;
        public:
            explicit wire(bit*);
            bool get_content() const{return this->content;};
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_WIRE_H
