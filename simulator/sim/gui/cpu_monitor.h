//
// Created by mihai-laptop on 6/27/23.
//

#ifndef SIMULATOR_CPU_MONITOR_H
#define SIMULATOR_CPU_MONITOR_H

#include "basic_output.h"
#include "cpu.h"

namespace sim {
    namespace objs{
        class cpu;
    }
    namespace gui {

        class cpu_monitor : public basic_output {
            objs::cpu& processor;
            QLabel* rga_lbl;
            QLabel* rga_val;
            QLabel* rgb_lbl;
            QLabel* rgb_val;
            QLabel* rgc_lbl;
            QLabel* rgc_val;
            QLabel* rgd_lbl;
            QLabel* rgd_val;

            QLabel* rmx_lbl;
            QLabel* rmx_val;
            QLabel* rsx_lbl;
            QLabel* rsx_val;
            QLabel* rbx_lbl;
            QLabel* rbx_val;
            QLabel* rpx_lbl;
            QLabel* rpx_val;

            QLabel* rgf_lbl;
            QLabel* rgf_val;
            QLabel* rgi_lbl;
            QLabel* rgi_val;
            QLabel* mem_lbl;
            QLabel* mem_val;

            QLabel* state_lbl;
            QLabel* state_val;

            QLabel* err_lbl;
            QLabel* err_val;

            QGridLayout* layout_main;

            QGridLayout* layout_8b;
            QGridLayout* layout_16b;



        public:
            explicit cpu_monitor(objs::cpu& processor);
            void update() override;
            void render() override;
        };

    } // sim
} // gui

#endif //SIMULATOR_CPU_MONITOR_H
