//
// Created by mihai-laptop on 3/13/23.
//

#include <iostream>
#include <thread>
#include <sys/time.h>
#include <unistd.h>
#include "environment.h"
#include "and_module.h"
#include "mux2x1.h"
#include "button.h"
#include "led.h"
#include "not_module.h"
#include "tiny_cpu.h"
#include "tiny_mem.h"
#include "cpu.h"
#include "memory.h"
#include "seven_seg.h"
#include "spinbox_adc.h"
#include "pwm_generator.h"
#include "decoder.h"
#include "translator.h"
#include "bootloader.h"


namespace sim {
    void environment::setup(const std::string &path) {
        topology_file = path;
        parse_phase();
        build_wire_phase();
        build_array_phase();
        build_component_phase();
        build_io_phase();
        config_phase();
    }

    void environment::start() {
        run_phase();
    }

    void environment::parse_phase() {
        topology = environment_generator::generate(topology_file);
    }

    void environment::build_wire_phase() {
        std::cout << "\ngenerating wires:\n";
        for(auto& wire : topology["wire_db"]){
            std::cout << "wire " << wire["name"] << "; level "<< wire["level"] <<'\n';
            auto* tmp = new objs::bit();
            tmp->set_expected_level(wire["level"]);
            wire_db[wire["name"]] = tmp;
        }
        wire_db["nil"] = &nil;
    }

    void environment::build_array_phase() {
        for(auto& array : topology["array_db"]){
            std::cout << "generating array " << array["name"] << "<"<< array["size"]<<">\n";
            auto* tmp = new objs::bit_array(array["size"]);
            for(int i = 0; i < tmp->get_size(); i++)
                tmp->connect(*wire_db[array["args"][i]], i);
            array_db[array["name"]] = tmp;
        }
    }

    void environment::build_component_phase() {
        std::cout << "\ngenerating components:\n";
        for(auto& component : topology["component_db"]){
            evaluable* tmp;
            if(component["type"] == "and_module"){
//                std::cout << "and_module " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ", "<< component["args"][2] << ")" << "; level "<< component["level"] <<'\n';
                auto* a = wire_db[component["args"][0]];
                auto* b = wire_db[component["args"][1]];
                auto* o = wire_db[component["args"][2]];
                tmp = new objs::and_module(*a,*b,*o, false);
            } else if(component["type"] == "not_module"){
//                std::cout << "not_module " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ")" << "; level "<< component["level"] <<'\n';
                auto* i = wire_db[component["args"][0]];
                auto* o = wire_db[component["args"][1]];
                tmp = new objs::not_module(*i, *o);
            } else if(component["type"] == "tiny_cpu") {
                auto& args = component["args"];
//                std::cout << "tiny_cpu " << component["name"] << "(" << args.dump() << "); level " << component["level"] << '\n';

                auto* addr_o = array_db[args[0]];
                auto* data_i = array_db[args[1]];
                auto* data_o = array_db[args[2]];

                auto* active = wire_db[args[3]];
                auto* rw = wire_db[args[4]];
                auto* ready = wire_db[args[5]];

                auto* port_i = array_db[args[6]];
                auto* port_o = array_db[args[7]];

                auto* interr = wire_db[args[8]];
                auto* CLK = wire_db[args[9]];
                auto* RST = wire_db[args[10]];

                tmp = new objs::tiny_cpu(*addr_o,*data_i,*data_o,*active,*rw,*ready,*port_i,*port_o,*interr,*CLK,*RST);
            } else if(component["type"] == "tiny_mem") {
                auto& args = component["args"];
//                std::cout << "tiny_mem " << component["name"] << "(" << args.dump() << "); level " << component["level"] << '\n';

                auto* addr_i = array_db[args[0]];
                auto* data_i = array_db[args[1]];
                auto* data_o = array_db[args[2]];

                auto* active = wire_db[args[3]];
                auto* rw = wire_db[args[4]];
                auto* ready = wire_db[args[5]];
                auto* CLK = wire_db[args[6]];
                auto* RST = wire_db[args[7]];

                tmp = new objs::tiny_mem(*addr_i,*data_i,*data_o,*active,*rw,*ready,*CLK,*RST);
            } else if(component["type"] == "mux2x1") {
//                std::cout << "mux2x1 " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ", "<< component["args"][2] << ", "<< component["args"][3] << ", " << ")" << "; level "<< component["level"] <<'\n';
                auto* a = wire_db[component["args"][0]];
                auto* b = wire_db[component["args"][1]];
                auto* s = wire_db[component["args"][2]];
                auto* o = wire_db[component["args"][3]];
                tmp = new objs::mux2x1(*a,*b,*s,*o);
            } else if(component["type"] == NAME_CPU){
                auto _tmp = objs::cpu::instantiate(wire_db,array_db,component);
                auto mon = new gui::cpu_monitor(*_tmp);
                tmp = _tmp;
                window_db[component["name"]] = mon->get_window();
                output_db[component["name"]] = mon;
            } else if(component["type"] == NAME_MEMORY){
                auto _tmp = objs::memory::instantiate(wire_db,array_db,component);
                tmp = _tmp;
                auto mon = new gui::bootloader(component["name"],*_tmp);
                window_db[component["name"]] = mon->get_window();
                input_db[component["name"]] = mon;
            } else if(component["type"] == NAME_PWM){
                tmp = objs::pwm_generator::instantiate(wire_db,array_db,component);
            } else if(component["type"] == NAME_DECODER){
                tmp = new objs::decoder(*array_db[component["args"][0]],*array_db[component["args"][1]]);
            } else if(component["type"] == NAME_TRANSLATOR){
                tmp = new objs::translator(*array_db[component["args"][0]],*array_db[component["args"][1]]);
            } else {
                throw std::runtime_error("invalid type \"" + std::string(component["type"]) + "\". how did you manage to pass all the fail safes?");
            }
            tmp->set_expected_level(component["level"]);
            component_db[component["name"]] = tmp;
        }
    }

    void environment::build_io_phase() {
        std::cout << "\ngenerating inputs:\n";
        moni = new gui::sim_monitor(this);
        window_db["sim_monitor"] = moni->get_window();
        output_db["sim_monitor"] = moni;
        for(auto& input : topology["io_db"]["inputs"]){
            basic_io* inp;
            if(input["type"] == "button"){
                std::cout << "button " << input["name"] << "(" << input["args"][0] << ")" <<'\n';
                inp =  new objs::button(input["name"],*wire_db[input["args"][0]]);
            } else if(input["type"] == NAME_SPINBOX_ADC){
                inp = objs::spinbox_adc::instantiate(wire_db,array_db,input);
            } else {
                throw std::runtime_error("invalid type \"" + std::string(input["type"]) + "\". how did you manage to pass all the fail safes?");
            }
            window_db[input["name"]] = inp->get_window();
            input_db[input["name"]] = inp;
        }
        std::cout << "\ngenerating outputs:\n";
        for(auto& output : topology["io_db"]["outputs"]){
            basic_output* out;
            if(output["type"] == "led"){
                std::cout << "led " << output["name"] << "(" << output["args"][0] << ")" <<'\n';
                out = new objs::led(output["name"], *wire_db[output["args"][0]]);
            } else if(output["type"] == NAME_SSD){
                out = new objs::seven_seg(*array_db[output["args"][0]],*wire_db[output["args"][1]]);
            } else {
                throw std::runtime_error("invalid type \"" + std::string(output["type"]) + "\". how did you manage to pass all the fail safes?");
            }
            output_db[output["name"]] = out;
            window_db[output["name"]] = out->get_window();
        }
        std::cout << "packing the sub-windows\n";
        for(auto& window : topology["window_db"]){
            auto* tmp = new QWidget();
            auto* grid = new QGridLayout();
            for(auto& arg : window["args"]){
                int row = arg["x"];
                int col = arg["y"];
                grid->addWidget(window_db[arg["content"]],row,col,Qt::AlignCenter);
                window_db[arg["content"]]->setParent(tmp);
                window_db.erase(arg["content"]);

            }
            window_db[window["name"]] = tmp;
            tmp->setLayout(grid);
        }
    }

    void environment::config_phase() {
        auto const& configs = topology["config_db"];
        if(configs.contains("sim_frequency_min")){
            sim_frequency_min = configs["sim_frequency_min"];
        }
        if(configs.contains("sim_frequency_max")){
            sim_frequency_max = configs["sim_frequency_max"];
        }
        if(configs.contains("frame_rate_cap")){
            frame_rate_cap = configs["frame_rate_cap"];
        }
        if(configs.contains("master_clk")){
            master_clk = wire_db[configs["master_clk"]];
        }

        // set up the evaluation lists here
        for (auto const &kvp: wire_db) {
            std::cout << "adding wire " << kvp.first << " at level " << kvp.second->get_expected_level() << '\n';
            evl.add_on_expected_level(kvp.second);
        }
        for(auto const& kvp : component_db){
            std::cout << "adding component " << kvp.first << " at level " << kvp.second->get_expected_level() << '\n';
            evl.add_on_expected_level(kvp.second);
        }

//        evl.commit();
    }
    void environment::run_phase() {
        //beginning of the overhauling.....
        auto micros = [](){
            timeval curTime{};
            gettimeofday(&curTime, nullptr);
            return curTime.tv_usec;
        };
        std::cout << "showing the windows\n";
//        for(auto input : input_db)
//            input.second->get_window()->show();
//        for(auto output : output_db)
//            output.second->get_window()->show();
        for(auto win : window_db)
            win.second->show();

        std::cout << "firing up the simulation\n";
        unsigned long frame_start;
        unsigned long delta;

        evl.full_eval();
        if(!master_clk){

            return;
        }
        while(!exit_flag){
            iterations = 0;
            frame_start = micros();
            delta = frame_start;
            moni->update();
            unsigned max_frame_time = 9.0e5f / (float)frame_rate_cap; // 90% of the frame time is reserved for the simulation
            while((run_flag || step_flag) && iterations < sim_frequency_max && (delta - frame_start < max_frame_time || iterations < sim_frequency_min)){
                for(auto& input : input_db)
                    input.second->update();
                master_clk->set_content(false);
                evl.eval();

                master_clk->set_content(true);
                evl.eval();

                for(auto& output : output_db)
                    output.second->update();

                iterations++;
                delta = micros();
                step_flag = false;
            }
            delta = micros();

            frame_time = delta - frame_start;

            for(const auto& output : output_db)
                output.second->render();
            long sleep_time = (1.0e6f / (float)frame_rate_cap) - frame_time;
            sleep_time = std::max(sleep_time,0l);
            usleep(sleep_time);
        }
        exit(0);
        std::cout << "simulation finished\n";
        clean_exit = true;
    }

    void environment::cleanup_phase() {
        for(auto const& kvp : window_db){
            kvp.second->close();
        }
        wire_db.erase("nil");
        for(auto const& kvp : wire_db){
            delete kvp.second;
        }
        wire_db.clear();

        for(auto const& kvp : array_db){
            delete kvp.second;
        }
        array_db.clear();

        for(auto const& kvp : component_db){
            delete kvp.second;
        }
        component_db.clear();

        for(auto const& kvp : input_db){
            delete kvp.second;
        }
        input_db.clear();

        for(auto const& kvp : output_db){
            delete kvp.second;
        }
        output_db.clear();

        topology.clear();
    }

    unsigned int environment::get_sim_frequency_min() const {
        return sim_frequency_min;
    }

    void environment::set_sim_frequency_min(unsigned int _sim_frequency_min) {
        this->sim_frequency_min = _sim_frequency_min;
    }

    unsigned int environment::get_sim_frequency_max() const {
        return sim_frequency_max;
    }

    void environment::set_sim_frequency_max(unsigned int _sim_frequency_max) {
        this->sim_frequency_max = _sim_frequency_max;
    }

    unsigned int environment::get_frame_rate_cap() const {
        return frame_rate_cap;
    }

    void environment::set_frame_rate_cap(unsigned int _frame_rate_cap) {
        this->frame_rate_cap = _frame_rate_cap;
    }

    bool environment::get_exit_flag() const {
        return exit_flag;
    }

    void environment::set_exit_flag(bool _exit_flag) {
        this->exit_flag = _exit_flag;
    }

    long environment::get_frame_time() const {
        return frame_time;
    }

    void environment::set_run_flag(bool _run_flag) {
        environment::run_flag = _run_flag;
    }

    void environment::set_step_flag(bool _step_flag) {
        environment::step_flag = _step_flag;
    }

    unsigned int environment::get_iterations() const {
        return iterations;
    }

    void environment::cleanup() {
        cleanup_phase();
    }

} // sim