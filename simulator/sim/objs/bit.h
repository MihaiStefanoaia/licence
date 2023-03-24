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
            explicit bit(bool = false);
            virtual ~bit();
            bool get_content() const;
            void set_content(bool);
            void eval() final {
                evaluable::eval();
            }
            void flag_for_eval(triggering* ev) final{
                evaluable::flag_for_eval(ev);
            }
        };

    } // sim
} // objs

#endif //SIMULATOR_BIT_H
