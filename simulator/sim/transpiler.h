#ifndef SIMULATOR_TRANSPILER_H

#define SIMULATOR_TRANSPILER_H

#include "json.hpp"
#include <string>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <utility>
#include "parser.hh"
#include "level_promise.h"
#include "cpu.h"
#include "memory.h"
#include "seven_seg.h"
#include "spinbox_adc.h"
#include "mux2x1.h"
#include "pwm_generator.h"
#include "decoder.h"
#include "translator.h"

// Give Flex the prototype of yylex we want ...
# define YY_DECL yy::parser::symbol_type yylex (sim::transpiler& trp)
// ... and declare it for the parser's sake.
YY_DECL;
namespace sim{
    class transpiler{
    private:
        transpiler();
        std::string file;
        nlohmann::json ret;
        int parse(const std::string& str);
        int result;
        void scan_begin ();
        void scan_end ();
        bool trace_scanning;
        bool trace_parsing;
    public:
        // these are the valid modules, and the list contains the width of each argument
        // the number of arguments can be inferred from the length of the list
        std::map<std::string,std::vector<unsigned int>> valid_modules;
        std::map<std::string,std::vector<unsigned int>> valid_inputs;
        std::map<std::string,std::vector<unsigned int>> valid_outputs;
        std::set<std::string> valid_configs;
        std::set<std::string> monitored;

        // databases of the read and written arguments of each
        std::set<std::string> positive_driven = {"tiny_cpu", NAME_CPU, NAME_PWM};
        std::set<std::string> negative_driven = {"tiny_mem","button", NAME_SSD, NAME_MEMORY, NAME_SPINBOX_ADC};
        std::set<std::string> always_driven = {};
        std::set<std::string> reactive_driven = {"and_module","not_module",NAME_MUX_2x1,NAME_DECODER, NAME_TRANSLATOR};
        std::map<std::string,std::vector<bool>> read_args = { // true is an input (read) argument, false is an output (written) argument
                {"and_module",{true,true,false}},
                {"not_module",{true,false}},
                {"tiny_cpu",{false,true,false,false,false,true,true,false,true,true,true}},
                {"tiny_mem",{true,true,false,true,true,false,true,true}},
                {"button",{false}},
                ARGS_DIR_CPU,
                ARGS_DIR_MEMORY,
                ARGS_DIR_SSD,
                ARGS_DIR_SPINBOX_ADC,
                ARGS_DIR_MUX_2x1,
                ARGS_DIR_PWM,
                ARGS_DIR_DECODER,
                ARGS_DIR_TRANSLATOR
        };

    private:
        void setup_dbs();
        void graph_analysis();
        static std::pair<std::string,unsigned int> resolve_access(nlohmann::json &dbs, nlohmann::json &lookup);
        static std::list<std::string> get_wires_from_argument(nlohmann::json &dbs, const std::string& lookup);
        bool is_monitored(nlohmann::json &dbs,const std::string &lookup);

        class node;

        static void prune_connections(std::map<std::string,node*> &graph);
        static void detect_cycles    (std::map<std::string,node*> &graph);
        static void determine_levels (std::map<std::string,node*> &graph);
        static void process_dfs_tree (std::map<node*,int> &visited, std::stack<node*> &stack);
        static void graph_cleanup(const std::map<std::string,node*>& graph);

    public:
        yy::location location;
        static nlohmann::json transpile(const std::string&);
        friend class yy::parser;
        void transpiler_init(){
            valid_modules = {{"and_module",{1,1,1}},
                             {"master_clk",{1}},
                             {"not_module",{1,1}},
                             {"tiny_cpu",{4,4,4,1,1,1,4,4,1,1,1}},
                             {"tiny_mem",{4,4,4,1,1,1,1,1}},
                             ARGS_SIZES_CPU,
                             ARGS_SIZES_MEMORY,
                             ARGS_SIZES_MUX_2x1,
                             ARGS_SIZES_PWM,
                             ARGS_SIZES_DECODER,
                             ARGS_SIZES_TRANSLATOR};
            valid_inputs  = {{"button",{1}},
                             ARGS_SIZES_SPINBOX_ADC};
            valid_outputs = {{"led",{1}},
                             ARGS_SIZES_SSD};
            valid_configs = {"sim_frequency_min", "sim_frequency_max", "frame_rate_cap"};
            monitored = {"tiny_mem","tiny_cpu", NAME_CPU, NAME_MEMORY};
        }
    };
}
#endif
