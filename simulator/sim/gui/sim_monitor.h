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
    class environment;
    namespace gui {

        class sim_monitor : public basic_output{
            sim::environment* env;
            QGridLayout* full_layout;

            QGridLayout* configs;

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

            QGridLayout* perf;

            QGridLayout* perf_lbl_grid;
            QLabel* fps_lbl;
            QLabel* fps_val;
            QLabel* prc_lbl;
            QLabel* prc_val;


            QGridLayout* ctrl_btn_grid;
            QPushButton* pause;
            QPushButton* step;
            QPushButton* run;
            QPushButton* exit;

            bool load_flag;
            bool submit_flag;
            bool pause_flag;
            bool step_flag;
            bool exit_flag;
            bool run_flag;
        public:
            explicit sim_monitor(sim::environment*);
            void update() override;
            void render() override;
        };

    } // sim
} // gui

#endif //SIMULATOR_SIM_MONITOR_H
