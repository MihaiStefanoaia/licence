//
// Created by Mihai - PC on 11/22/2022.
//

#include "master_clk.h"

namespace sim {
    master_clk *master_clk::get_instance() {
        if(master_clk::instance == nullptr){
            master_clk::instance = new master_clk();
        }
        return master_clk::instance;
    }

    master_clk::master_clk():triggering(){}

    void master_clk::step() {
        master_clk::get_instance()->eval();
    }

    void master_clk::eval() {
        for(auto ev : *this->on_neg){
            ev->eval();
        }
        for(auto ev : *this->on_pos){
            ev->eval();
        }
    }
} // sim