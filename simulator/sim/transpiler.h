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
            std::vector<std::string> arguments;
            std::map<node*,bool> dependencies; // the value at the map is whether the dependency is critical

            enum drive_type {NONE, NEGATIVE, POSITIVE, BOTH, REACTIVE};

            drive_type driven_when = NONE;

            struct drive_struct {
                bool at_neg = false;
                bool at_pos = false;
            } driven;
            bool visited = false;
            node(std::string name, std::string n_type){
                this->name = std::move(name);
                this->n_type = std::move(n_type);
            };
            void drive(drive_type on_what) {
                if(driven.at_pos || driven.at_neg) //wires can only be driven by one module
                    throw std::runtime_error("Failed: wire " + name + " is being driven by multiple objects");
                driven.at_neg = (on_what == NEGATIVE) || (on_what == BOTH);
                driven.at_pos = (on_what == POSITIVE) || (on_what == BOTH);
            }
            void add_dependency(node* dpd){
                if(!dependencies.count(dpd))
                    dependencies[dpd] = true;
            }
            bool is_driven() const{
                return driven.at_pos || driven.at_neg;
            }
            bool filter_dependency(node* dpd){ // returns the status of the dependency after the process
                if(!dependencies.count(dpd))
                    throw std::runtime_error("dependency "+ dpd->name + " is not found in module " + name);
                if(!dpd->is_driven())
                    return true;
                if(!dependencies[dpd])
                    return false;
                switch (driven_when) {
                    case POSITIVE:
                        if(dpd->driven.at_pos)
                            return true;
                        dependencies[dpd] = false;
                        return false;
                    case NEGATIVE:
                        if(dpd->driven.at_neg)
                            return true;
                        dependencies[dpd] = false;
                        return false;
                    case BOTH:
                    case REACTIVE:
                    case NONE:
                        return true;
                }
            }
            bool detect_dependency_changes(){
                for(auto& dep : dependencies){
                    if(dep.second && !filter_dependency(dep.first))
                        return true;
                }
                return false;
            }

            bool only_critical_dependencies(){
                if(dependencies.empty())
                    return true;
                if(driven_when == REACTIVE)
                    return true;
                bool ret_ = true;
                for(auto& dep : dependencies){
                    ret_ &= !dep.second;
                }
                return ret_;
            }

            void make_unreactive(){
                if(driven_when == REACTIVE && only_critical_dependencies()){
                    auto new_drive = REACTIVE;
                    for(auto dpd : dependencies){
                        if( (new_drive == POSITIVE && dpd.first->driven_when == NEGATIVE) ||
                            (new_drive == NEGATIVE && dpd.first->driven_when == POSITIVE) ||
                            dpd.first->driven_when == BOTH)
                            new_drive = BOTH;
                        else
                            new_drive = dpd.first->driven_when;
                    }
                    driven_when = new_drive;
                }
            }

            friend std::ostream& operator<<(std::ostream& o, node& n){
                o << "Node:\n";
                o << "Name: " << n.name << '\n';
                o << "Type: " << n.n_type << '\n';
                o << "Driven on: ";
                switch (n.driven_when) {

                    case NONE:
                        o << "none\n";
                        break;
                    case NEGATIVE:
                        o << "negative\n";
                        break;
                    case POSITIVE:
                        o << "positive\n";
                        break;
                    case BOTH:
                        o << "both\n";
                        break;
                    case REACTIVE:
                        o << "reactive\n";
                        break;
                }
                o << "Arguments: [";
                for(const auto& arg : n.arguments){
                    o << arg << ' ';
                }
                o << "]\n";
                o << "Critical dependencies: [";
                for(const auto& arg : n.dependencies){
                    if(arg.second)
                        o << arg.first->name << ' ';
                }
                o << "]\n";
                return o;
            }
        };
    public:
        yy::location location;
        static nlohmann::json transpile(const std::string&);
        friend class yy::parser;
    };
}
#endif