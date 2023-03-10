//
// Created by Mihai - PC on 11/15/2022.
//

#ifndef SIMULATOR_BIT_H
#define SIMULATOR_BIT_H

#include "../triggering.h"

namespace sim {
    namespace objs {

        class bit : public triggering, public evaluable{
        private:
            bool content;
        public:
            explicit bit(bool);
            bool get_content() const;
            void set_content(bool);
        };

    } // sim
} // objs

#endif //SIMULATOR_BIT_H
