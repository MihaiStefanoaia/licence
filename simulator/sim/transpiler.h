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
            enum drive_type {NONE, NEGATIVE, POSITIVE, BOTH, REACTIVE};

            std::string name;
            std::string n_type;
            drive_type driven = REACTIVE;
            std::set<node*> incoming;
            std::set<node*> outgoing;
            level_promise level = level_promise(level_promise::NONE,{});

            std::vector<std::string> arguments;


            bool visited = false;
            node(std::string name, std::string n_type){
                this->name = std::move(name);
                this->n_type = std::move(n_type);
            };
            void drive(drive_type on_what) {
                driven = on_what;
            }
            void connect_incoming(node* other){
                if(n_type == "wire" && !(incoming.empty() || incoming.count(other)))
                    throw std::runtime_error("wires cannot be driven by more than one source");
                incoming.insert(other);
                other->outgoing.insert(this);
            }

            void connect_outgoing(node* other){
                if(other->n_type == "wire" && !(other->incoming.empty() || other->incoming.count(this)))
                    throw std::runtime_error("wires cannot be driven by more than one source");
                outgoing.insert(other);
                other->incoming.insert(this);
            }

            bool is_prunable(node* other) const{
                if( (driven == NEGATIVE && other->driven == POSITIVE) ||
                    (driven == POSITIVE && other->driven == NEGATIVE) ||
                    other->driven == NONE)
                    return true;
                return false;
            }

            void prune(node* other){
                if(incoming.count(other)){
                    incoming.erase(other);
                    other->outgoing.erase(this);
                }
                if(outgoing.count(other)){
                    outgoing.erase(other);
                    other->incoming.erase(this);
                }
            }

            bool make_unreactive(){
                if(driven != REACTIVE)
                    return false;

                auto new_drive = NONE;
                for(auto inc : incoming){
                    if(inc->driven == REACTIVE)
                        return false;
                    if(inc->driven == NONE)
                        continue;
                    if((new_drive == POSITIVE && inc->driven == NEGATIVE) ||
                       (new_drive == NEGATIVE && inc->driven == POSITIVE) ||
                        new_drive == BOTH || inc->driven == BOTH)
                        new_drive = BOTH;
                    else
                        new_drive = inc->driven;
                }
                driven = new_drive;
                return true;
            }

            friend std::ostream& operator<<(std::ostream& o, node& n){
                o << "Node:\n";
                o << "Name: " << n.name << '\n';
                o << "Type: " << n.n_type << '\n';
                o << "Driven on: ";
                switch (n.driven) {

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
                o << "Incoming connections: [";
                for(const auto& inc : n.incoming){
                    o << inc->name << ' ';
                }
                o << "]\n";
                o << "Outgoing connections: [";
                for(const auto& otg : n.outgoing){
                    o << otg->name << ' ';
                }
                o << "]\n";
                return o;
            }
        };

        static void prune_connections(std::map<std::string,node*> &graph);
        static void detect_cycles    (std::map<std::string,node*> &graph);
        static void determine_levels (std::map<std::string,node*> &graph);
        static void process_dfs_tree (std::map<node*,int> &visited, std::stack<node*> &stack);
        static void graph_cleanup(const std::map<std::string,node*>& graph);

    public:
        yy::location location;
        static nlohmann::json transpile(const std::string&);
        friend class yy::parser;
    };
}
#endif
