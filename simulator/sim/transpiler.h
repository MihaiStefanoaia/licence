#ifndef SIMULATOR_TRANSPILER_H

#define SIMULATOR_TRANSPILER_H

#include "json.hpp"
#include <string>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "parser.hh"
#include "level_promise.h"

// Give Flex the prototype of yylex we want ...
# define YY_DECL yy::parser::symbol_type yylex (sim::transpiler& trp)
// ... and declare it for the parser's sake.
YY_DECL;
namespace sim{
    class transpiler{
        transpiler();
        std::string file;
        nlohmann::json ret;
        int parse(const std::string& str);
        int result;
        void scan_begin ();
        void scan_end ();
        bool trace_scanning;
        bool trace_parsing;

        // these are the valid modules, and the list contains the width of each argument
        // the number of arguments can be inferred from the length of the list
        std::map<std::string,std::vector<unsigned int>> valid_modules = {{"and_module",{1,1,1}},{"master_clk",{1}},{"not_module",{1,1}},{"tiny_cpu",{4,4,4,1,1,1,4,4,1,1,1}},{"tiny_mem",{4,4,4,1,1,1,1,1}}};
        std::map<std::string,std::vector<unsigned int>> valid_inputs  = {{"button",{1}}};
        std::map<std::string,std::vector<unsigned int>> valid_outputs = {{"led",{1}}};

        std::set<std::string> valid_configs = {"sim_frequency_min", "sim_frequency_max", "frame_rate_cap"};

        void setup_dbs();
        void graph_analysis();
        static std::pair<std::string,unsigned int> resolve_access(nlohmann::json &dbs, nlohmann::json &lookup);
        static std::list<std::string> get_wires_from_argument(nlohmann::json &dbs, const std::string& lookup);
        class node{
        public:
            std::string name;
            std::string n_type;
            level_promise level = level_promise(level_promise::NONE,{});
            std::map<node*,bool> dependencies; // the value at the map is whether the dependency is critical
            enum drive_type {POSITIVE, NEGATIVE, BOTH, REACTIVE};

            drive_type driven_when;

            struct drive_struct {
                bool at_neg = false;
                bool at_pos = false;
            } driven;
            bool visited = false;
            node(std::string name, std::string n_type){
                this->name = std::move(name);
                this->n_type = std::move(n_type);
            };
            void drive(unsigned int on_what) {
                if(driven.at_pos || driven.at_neg) //wires can only be driven by one module
                    throw std::runtime_error("Failed: wire " + name + " is being driven by multiple objects");
                driven.at_neg = (on_what == 1) || (on_what == 3);
                driven.at_pos = (on_what == 2) || (on_what == 3);
            }
            void add_dependency(node* dpd){
                if(!dependencies.count(dpd))
                    dependencies[dpd] = true;
            }
            bool is_driven() const{
                return driven.at_pos || driven.at_neg;
            }
            bool scrutinize_dependency(node* dpd){ // returns the status of the dependency after the
                if(!dependencies.count(dpd))
                    throw std::runtime_error("dependency "+ dpd->name + " is not found in module " + name);
                if(!dpd->is_driven())
                    return true;
                switch (driven_when) {
                    case POSITIVE:
                        if(dpd->driven.at_pos)
                            return true;
                        dependencies[dpd] = false;
                        return false;
                    case NEGATIVE:
                        if(dpd->driven.at_neg)
                            return true;
                        return false;
                    case BOTH:
                    case REACTIVE:
                        return true;
                }
            }
            bool only_critical_dependencies(){
                bool ret_ = true;
                for(auto& dep : dependencies){
                    ret_ &= (dep.first->is_driven() && scrutinize_dependency(dep.first));
                }
                return ret_;
            }
        };
    public:
        yy::location location;
        static nlohmann::json transpile(const std::string&);
        friend class yy::parser;
    };
}
#endif