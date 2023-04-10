//
// Created by mihai-laptop on 3/13/23.
//

#ifndef SIMULATOR_ENVIRONMENT_H
#define SIMULATOR_ENVIRONMENT_H

#include "evaluation_list.h"
#include <map>
#include "objs/bit.h"
#include "../json.hpp"
#include "environment_generator.h"
#include "basic_input.h"
#include "basic_output.h"
#include <mutex>

namespace sim {

    class environment {
    protected:
        nlohmann::json topology;
        std::map<std::string,objs::bit*> wire_db;
        std::map<std::string,evaluable*> component_db;
        std::map<std::string,basic_input*> input_db;
        std::map<std::string,basic_output*> output_db;
        std::map<std::string,int> control_values;
        std::mutex run_lock;
        objs::bit master_clk;
        evaluation_list evl;
        std::string topology_file = "example";
        unsigned int min_cycles = 1;
        bool clean_exit = false;
        bool has_master_clk = false;

        void parse_phase();
        void build_wire_phase();
        void build_module_phase();
        void build_io_phase();
        void pre_run_phase();
        void run_phase();
        void cleanup_phase();
    public:
        void start(const std::string& = "example");
        inline void change_input(const std::string&,bool);
    };

} // sim

#endif //SIMULATOR_ENVIRONMENT_H
