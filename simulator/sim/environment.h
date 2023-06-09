//
// Created by mihai-laptop on 3/13/23.
//

#ifndef SIMULATOR_ENVIRONMENT_H
#define SIMULATOR_ENVIRONMENT_H

#include "evaluation_list.h"
#include <map>
#include "bit.h"
#include "json.hpp"
#include "environment_generator.h"
#include "basic_input.h"
#include "basic_output.h"
#include "bit_array.h"
#include "none.h"
#include "sim_monitor.h"
#include <mutex>

namespace sim {

    class environment {
    protected:
        gui::sim_monitor* monitor;
        nlohmann::json topology;
        objs::none nil = objs::none();
        std::map<std::string,objs::bit*> wire_db;
        std::map<std::string,objs::bit_array*> array_db;
        std::map<std::string,evaluable*> component_db;
        std::map<std::string,basic_io*> input_db;
        std::map<std::string,basic_output*> output_db;
        std::map<std::string,int> config_db;
        std::mutex run_lock;
        std::mutex io_lock;
        objs::bit* master_clk;
        evaluation_list evl;
        std::string topology_file = "example";
        unsigned int sim_frequency_min = 1;
        unsigned int sim_frequency_max = 1000;
    public:
        unsigned int get_sim_frequency_min() const;

        void set_sim_frequency_min(unsigned int sim_frequency_min);

        unsigned int get_sim_frequency_max() const;

        void set_sim_frequency_max(unsigned int _sim_frequency_max);

        unsigned int get_frame_rate_cap() const;

        void set_frame_rate_cap(unsigned int _frame_rate_cap);

        bool get_exit_flag() const;

        void set_exit_flag(bool _exit_flag);

    protected:
        unsigned int reactive_only = 0;
        unsigned int frame_rate_cap = -1;
        bool clean_exit = false;
        bool has_master_clk = false;
        bool exit_flag = false;


        void parse_phase();
        void build_wire_phase();
        void build_array_phase();
        void build_component_phase();
        void build_io_phase();
        void config_phase();
        void run_phase();
        void cleanup_phase();
    public:
        void start(const std::string& = "example");
        void operator()(const std::string& s = "example"){
            start(s);
        }
        friend class gui::sim_monitor;
    };

} // sim

#endif //SIMULATOR_ENVIRONMENT_H
