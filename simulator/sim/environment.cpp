//
// Created by mihai-laptop on 3/13/23.
//

#include <iostream>
#include <thread>
#include "environment.h"
#include "and_module.h"
#include "mux2x1.h"
#include "button.h"
#include "led.h"

namespace sim {
    void environment::start(const std::string& path) {
        do {
            topology_file = path;
            parse_phase();
            build_wire_phase();
            build_module_phase();
            build_io_phase();
            pre_run_phase();
            run_phase();
            cleanup_phase();
        } while(!clean_exit);
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
    }

    void environment::build_module_phase() {
        std::cout << "\ngenerating components:\n";
        for(auto& component : topology["component_db"]){
            evaluable* tmp;
            if(component["type"] == "and_module"){
                std::cout << "and_module " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ", "<< component["args"][2] << ")" << "; level "<< component["level"] <<'\n';
                auto* a = wire_db[component["args"][0]];
                auto* b = wire_db[component["args"][1]];
                auto* o = wire_db[component["args"][2]];
                tmp = new objs::and_module(*a,*b,*o, false);
            } else {
                std::cout << "mux2x1 " << component["name"] << "("<< component["args"][0] << ", "<< component["args"][1] << ", "<< component["args"][2] << ", "<< component["args"][3] << ", " << ")" << "; level "<< component["level"] <<'\n';
                auto* a = wire_db[component["args"][0]];
                auto* b = wire_db[component["args"][1]];
                auto* s = wire_db[component["args"][2]];
                auto* o = wire_db[component["args"][3]];
                tmp = new objs::mux2x1(*a,*b,*s,*o);
            }
            tmp->set_expected_level(component["level"]);
            component_db[component["name"]] = tmp;
        }
    }

    void environment::build_io_phase() {
        std::cout << "\ngenerating buttons:\n";
        for(auto& input : topology["io_db"]["inputs"]){
            std::cout << "button " << input["name"] << "(" << input["args"][0] << ")" <<'\n';
            auto* btn =  new objs::button(*wire_db[input["args"][0]]);
            input_db[input["name"]] = btn;
        }
        std::cout << "\ngenerating leds:\n";
        for(auto& output : topology["io_db"]["outputs"]){
            std::cout << "led " << output["name"] << "(" << output["args"][0] << ")" <<'\n';
            auto* btn =  new objs::led(*wire_db[output["args"][0]]);
            output_db[output["name"]] = btn;
        }
    }

    void environment::pre_run_phase() {
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
        std::string cli_input;
        std::string cmd, target;
        while(cli_input != "exit") {
//        system("clear");
            for(auto const& kvp : input_db){
                std::cout << kvp.first << " : " << kvp.second->output.get_content() << '\n';
                kvp.second->update();
            }

            for(auto const& kvp : output_db){
                std::cout << kvp.first << " : ";
                kvp.second->render();
            }

            for(auto const& kvp : wire_db){
                std::cout << kvp.first << " : " << kvp.second->get_content() << '\n';
            }

            char buf[100];
            std::cout << ">>>";
            std::cin.getline(buf, 100);
            cli_input = buf;
            cmd = cli_input.substr(0, cli_input.find(' '));
            target = cli_input.substr(cli_input.find(' ') + 1, std::string::npos);
//            std::cout << cmd << "---" << target << std::endl;
            if (cmd == "set") {
                change_input(target,true);
            } else if (cmd == "unset") {
                change_input(target,false);
            } else if (cli_input == "exit") {
                std::cout << "exiting\n";
                break;
            } else {
                std::cout << "invalid command (set or unset only)\n";
                continue;
            }

            for(auto const& kvp : input_db){
                kvp.second->update();
            }

            evl.eval();

            for(auto const& kvp : output_db){
                kvp.second->update();
            }
        }
        clean_exit = true;
    }

    void environment::cleanup_phase() {
        for(auto const& kvp : wire_db){
            delete kvp.second;
        }
        wire_db.clear();

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

    inline void environment::change_input(const std::string& name, bool value) {
        if(name == "exit") {
            clean_exit = true;

        }
        if(input_db.count(name))
            input_db[name]->set_value(value);
    }
} // sim