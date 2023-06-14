//
// Created by mihai-pc on 6/14/23.
//

#ifndef SIMULATOR_SIM_MONITOR_H
#define SIMULATOR_SIM_MONITOR_H

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "basic_input.h"
#include "basic_output.h"
#include "environment.h"

namespace sim {
    namespace gui {

        class sim_monitor : public basic_input, public basic_output{
            sim::environment* env;
            QGridLayout* full_layout;

            QWidget* configs;
            QGridLayout* config_grid;

            QGridLayout* config_spb_grid;
            QLabel*   min_itr_lbl;
            QSpinBox* min_itr_spb;
            QLabel*   max_itr_lbl;
            QSpinBox* max_itr_spb;
            QLabel*   max_fps_lbl;
            QSpinBox* max_fps_spb;

            QGridLayout* config_btn_grid;
            QPushButton* reload;
            QPushButton* submit;

            QWidget* perf;
            QGridLayout* perf_grid;

            QLabel* fps_lbl;
            QLabel* fps_val;
            QLabel* prc_lbl;
            QLabel* prc_val;

            QPushButton* pause;
            QPushButton* step;
            QPushButton* run;
            QPushButton* exit;
        public:
            sim_monitor(sim::environment*);
        };

    } // sim
} // gui

#endif //SIMULATOR_SIM_MONITOR_H
