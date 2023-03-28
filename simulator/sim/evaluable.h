//
// Created by Mihai - PC on 11/15/2022.
//

#ifndef SIMULATOR_EVALUABLE_H
#define SIMULATOR_EVALUABLE_H


#include <cassert>
#include "triggering.h"

namespace sim {
    class triggering;

    class evaluable {
    protected:
        triggering* flag = nullptr;
        int expected_level = 0;
    public:
        evaluable() = default;
        virtual ~evaluable() = default;
        virtual void eval(){if(this->flag){this->flag = nullptr;}}
        virtual void flag_for_eval(triggering* ev){ if(this->flag == nullptr){this->flag = ev;}}
        bool is_flagged() const{return this->flag != nullptr;}
        void set_expected_level(int new_level){assert(expected_level == 0); this->expected_level = new_level;}
        int get_expected_level() const{return this->expected_level;}
    };

} // sim

#endif //SIMULATOR_EVALUABLE_H
