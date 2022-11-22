//
// Created by Mihai - PC on 11/15/2022.
//

#ifndef SIMULATOR_EVALUABLE_H
#define SIMULATOR_EVALUABLE_H

namespace sim {

    class evaluable {
    protected:
        bool flag = false;
        int expected_level = 0;
    public:
        evaluable() = default;
        virtual void eval(){if(this->flag){this->flag = false;}}
        void flag_for_eval(){ this->flag = true;}
        bool is_flagged() const{return this->flag;}
        void set_expected_level(int new_level){this->expected_level = new_level;}
        int get_expected_level() const{return this->expected_level;}
    };

} // sim

#endif //SIMULATOR_EVALUABLE_H
