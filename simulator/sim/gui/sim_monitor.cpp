//
// Created by mihai-pc on 6/3/23.
//

#include "sim_monitor.h"

namespace sim {
    namespace gui {
        sim_monitor::sim_monitor(environment &parent) : parent(parent){

        }

        void sim_monitor::update() {
            if(flagged){
                parent.sim_frequency_min = commit_freq_min;
            }
        }
    } // sim
} // gui