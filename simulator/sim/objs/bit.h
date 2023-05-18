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
            ~bit() override;
            bool get_content() const;
            virtual void set_content(bool);
            void flag_for_eval(triggering* ev) final{
                evaluable::flag_for_eval(ev);
            }
        };

    } // sim
} // objs

#endif //SIMULATOR_BIT_H
