//
// Created by Mihai - PC on 11/15/2022.
//

#include <list>
#include "triggering.h"

namespace sim {
    void triggering::add_trigger(evaluable* t, trig_type on_what) {
        if(on_what == trig_type::BOTH) {
            this->on_pos.push_back(t);
            this->on_neg.push_back(t);
        } else if(on_what == trig_type::NEGATIVE) {
            this->on_neg.push_back(t);
        } else if(on_what == trig_type::POSITIVE) {
            this->on_pos.push_back(t);
        }
    }

    void triggering::on_value_change(trig_type on_what) {
        if(on_what == trig_type::POSITIVE){
            for(auto el : this->on_pos){
                el->flag_for_eval(this);
            }
        } else if(on_what == trig_type::NEGATIVE){
            for(auto el : this->on_neg){
                el->flag_for_eval(this);
            }
        }
    }

    triggering::triggering()= default;

    triggering::~triggering() = default;
} // sim