#ifndef SIMULATOR_TRIGGERING_H
#define SIMULATOR_TRIGGERING_H

#include <vector>
#include <list>
#include "evaluable.h"

namespace sim {

    class triggering : public evaluable{
    protected:
        std::list<evaluable*>* on_pos; //saves all the objects this object will trigger
        std::list<evaluable*>* on_neg;
    public:
        triggering();
        ~triggering();
        enum trig_type{
            POSITIVE, NEGATIVE, BOTH
        };
        void add_trigger(evaluable*, trig_type);
    protected:
        void on_value_change(trig_type);
    };

} // sim

#endif //SIMULATOR_TRIGGERING_H
