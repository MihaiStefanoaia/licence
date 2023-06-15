//
// Created by mihai-pc on 6/14/23.
//

#include "sim_monitor.h"

namespace sim {
    namespace gui {
        sim_monitor::sim_monitor(sim::environment *env) {
            this->env = env;
            window = new QWidget();

            full_layout = new QGridLayout(window);

            configs = new QGridLayout();

            config_spb_grid = new QGridLayout();

            min_itr_lbl = new QLabel("Minimum iterations:");
            min_itr_spb = new QSpinBox();
            min_itr_spb->setRange(1,100000);

            max_itr_lbl = new QLabel("Maximum iterations:");
            max_itr_spb = new QSpinBox();
            max_itr_spb->setRange(1,1000000);

            max_fps_lbl = new QLabel("Max fps:");
            max_fps_spb = new QSpinBox();
            max_fps_spb->setMinimum(30);

            config_spb_grid->addWidget(min_itr_lbl,0,0, Qt::AlignLeft);
            config_spb_grid->addWidget(min_itr_spb,0,1, Qt::AlignRight);
            config_spb_grid->addWidget(max_itr_lbl,1,0, Qt::AlignLeft);
            config_spb_grid->addWidget(max_itr_spb,1,1, Qt::AlignRight);
            config_spb_grid->addWidget(max_fps_lbl,2,0, Qt::AlignLeft);
            config_spb_grid->addWidget(max_fps_spb,2,1, Qt::AlignRight);

            config_btn_grid = new QGridLayout();
            reload = new QPushButton("reload");
            submit = new QPushButton("submit");

            config_btn_grid->addWidget(reload,0,0);
            config_btn_grid->addWidget(submit,0,1);

            configs->addLayout(config_spb_grid,0,0);
            configs->addLayout(config_btn_grid,1,0);

            perf = new QGridLayout();

            perf_lbl_grid = new QGridLayout();

            fps_lbl = new QLabel("FPS:");
            fps_val = new QLabel(std::to_string(env->get_frame_rate_cap()).c_str());
            prc_lbl = new QLabel("Cycles executed: ");
            prc_val = new QLabel("0 (0%)");

            perf_lbl_grid->addWidget(fps_lbl,0,0,Qt::AlignLeft);
            perf_lbl_grid->addWidget(fps_val,0,1,Qt::AlignRight);
            perf_lbl_grid->addWidget(prc_lbl,1,0,Qt::AlignLeft);
            perf_lbl_grid->addWidget(prc_val,1,1,Qt::AlignRight);

            ctrl_btn_grid = new QGridLayout();

            pause = new QPushButton("pause");
            step = new QPushButton("step");
            run = new QPushButton("run");
            exit = new QPushButton("exit");

            ctrl_btn_grid->addWidget(run,0,0);
            ctrl_btn_grid->addWidget(pause,0,1);
            ctrl_btn_grid->addWidget(exit,1,0);
            ctrl_btn_grid->addWidget(step,1,1);

            perf->addLayout(perf_lbl_grid,0,0);
            perf->addLayout(ctrl_btn_grid,1,0);

            full_layout->addLayout(configs,0,0);
            full_layout->addLayout(perf,1,0);

            window->setLayout(full_layout);

            // functions for each button

            auto load_func = [this](){
                this->load_flag = true;
            };

            load_func();

            QPushButton::connect(reload,&QPushButton::clicked,load_func);

            auto submit_func = [this](){
                this->submit_flag = true;
            };

            QPushButton::connect(submit,&QPushButton::clicked,submit_func);

            auto pause_func = [this](){
                this->pause_flag = true;
            };

            QPushButton::connect(pause,&QPushButton::clicked,pause_func);

            auto exit_func = [this](){
                this->exit_flag = true;
            };

            QPushButton::connect(exit,&QPushButton::clicked,exit_func);

            auto run_func = [this](){
                this->run_flag = true;
            };

            QPushButton::connect(run,&QPushButton::clicked,run_func);

            auto step_func = [this](){
                this->step_flag = true;
            };

            QPushButton::connect(step,&QPushButton::clicked,step_func);
        }

        void sim_monitor::update() {
            if(load_flag){
                auto min_itr = this->env->get_sim_frequency_min();
                min_itr_spb->setValue(min_itr);
                auto max_itr = this->env->get_sim_frequency_max();
                max_itr_spb->setValue(max_itr);
                auto fps = this->env->get_frame_rate_cap();
                max_fps_spb->setValue(fps);

                load_flag = false;
            }
            if(submit_flag){
                this->env->set_sim_frequency_min(min_itr_spb->value());
                this->env->set_sim_frequency_max(max_itr_spb->value());
                this->env->set_frame_rate_cap(max_fps_spb->value());

                submit_flag = false;
            }
            if(pause_flag){
                this->env->set_run_flag(false);
                pause_flag = false;
            }
            if(step_flag){
                this->env->set_run_flag(false);
                this->env->set_step_flag(true);

                step_flag = false;
            }
            if(exit_flag){
                this->env->set_exit_flag(true);

                exit_flag = false;
            }

            if(run_flag){
                this->env->set_run_flag(true);

                run_flag = false;
            }
        }

        void sim_monitor::render() {
            auto fps = env->get_frame_rate_cap();
            auto frame_time = env->get_frame_time();
            long max_ft = 1e6l / fps;
            float real_fps = 1.0e6f / std::max(max_ft,frame_time);
            fps_val->setText(std::to_string(real_fps).c_str());

            auto itr = env->get_iterations();
            auto max_itr = env->get_sim_frequency_max();
            std::string s;
            s += std::to_string(itr) + " (";
            s += std::to_string((int)((float)itr * 100/(float)max_itr)) + "%)";
            prc_val->setText(s.c_str());
        }
    } // sim
} // gui