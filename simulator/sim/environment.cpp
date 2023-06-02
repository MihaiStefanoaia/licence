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


namespace sim {
    void environment::start(const std::string& path) {
        do {
            topology_file = path;
            parse_phase();
            build_wire_phase();
            build_array_phase();
            build_component_phase();
            build_io_phase();
            config_phase();
            run_phase();
            cleanup_phase();
        } while(!clean_exit);
        std::cout << "exiting cleanly\n";
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
                std::cout << "and_module " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ", "<< component["args"][2] << ")" << "; level "<< component["level"] <<'\n';
                auto* a = wire_db[component["args"][0]];
                auto* b = wire_db[component["args"][1]];
                auto* o = wire_db[component["args"][2]];
                tmp = new objs::and_module(*a,*b,*o, false);
            } else if(component["type"] == "not_module"){
                std::cout << "not_module " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ")" << "; level "<< component["level"] <<'\n';
                auto* i = wire_db[component["args"][0]];
                auto* o = wire_db[component["args"][1]];
                tmp = new objs::not_module(*i, *o);
            } else if(component["type"] == "tiny_cpu") {
                auto& args = component["args"];
                std::cout << "tiny_cpu " << component["name"] << "(" << args.dump() << "); level " << component["level"] << '\n';

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
                std::cout << "tiny_mem " << component["name"] << "(" << args.dump() << "); level " << component["level"] << '\n';

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
                std::cout << "mux2x1 " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ", "<< component["args"][2] << ", "<< component["args"][3] << ", " << ")" << "; level "<< component["level"] <<'\n';
                auto* a = wire_db[component["args"][0]];
                auto* b = wire_db[component["args"][1]];
                auto* s = wire_db[component["args"][2]];
                auto* o = wire_db[component["args"][3]];
                tmp = new objs::mux2x1(*a,*b,*s,*o);
            } else {
                throw std::runtime_error("invalid type. how did you manage to pass all the fail safes?");
            }
            tmp->set_expected_level(component["level"]);
            component_db[component["name"]] = tmp;
        }
    }

    void environment::build_io_phase() {
        std::cout << "\ngenerating buttons:\n";
        for(auto& input : topology["io_db"]["inputs"]){
            std::cout << "button " << input["name"] << "(" << input["args"][0] << ")" <<'\n';
            auto* btn =  new objs::button(input["name"],*wire_db[input["args"][0]]);
            input_db[input["name"]] = btn;
        }
        std::cout << "\ngenerating leds:\n";
        for(auto& output : topology["io_db"]["outputs"]){
            std::cout << "led " << output["name"] << "(" << output["args"][0] << ")" <<'\n';
            auto* btn =  new objs::led(output["name"],*wire_db[output["args"][0]]);
            output_db[output["name"]] = btn;
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
        if(configs.contains("reactive_only")){
            reactive_only = configs["reactive_only"];
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
    }
    void environment::run_phase() {
        //beginning of the overhauling.....
        auto millis = [](){
            timeval curTime{};
            gettimeofday(&curTime, nullptr);
            return (unsigned long)curTime.tv_usec / 1000;
        };

        for(auto input : input_db)
            input.second->get_window()->show();
        for(auto output : output_db)
            output.second->get_window()->show();

        unsigned long frame_start;
        unsigned long delta;

        unsigned int iterations;
        std::cout << wire_db["m_clk"] << " " << master_clk << "\n";
        while(!exit_flag){
            iterations = 0;
            frame_start = millis();
            delta = frame_start;

            while(iterations < sim_frequency_max && (delta - frame_start < 1000.0f / (float)frame_rate_cap || iterations < sim_frequency_min)){
                master_clk->set_content(!master_clk->get_content());
                evl.eval();
                iterations++;
                for(auto input : input_db)
                    input.second->update();
                for(auto output : output_db)
                    output.second->update();
                delta = millis();
            }

            long sleep_time = (1000.0f / frame_rate_cap) - (delta - frame_start);
            sleep_time = std::max(sleep_time,0l);
            usleep(1000 * sleep_time);
            for(const auto& output : output_db)
                output.second->render();
        }
        clean_exit = true;
    }

    void environment::cleanup_phase() {
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
} // sim