//
// Created by mihai-pc on 6/3/23.
//

#ifndef SIMULATOR_SIM_MONITOR_H
#define SIMULATOR_SIM_MONITOR_H

#include "basic_io.h"
#include "environment.h"

namespace sim {
    namespace gui {

        class sim_monitor : public basic_io{
            sim::environment& parent;
            unsigned int commit_freq_max = 0;
            unsigned int commit_freq_min = 0;
            unsigned int commit_frame_rate_cap = 0;
            unsigned int commit_exit = 0;
        public:
            sim_monitor(sim::environment& parent);
            void update() override;
        };

    } // sim
} // gui

#endif //SIMULATOR_SIM_MONITOR_H
