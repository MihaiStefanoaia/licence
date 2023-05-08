#ifndef SIMULATOR_TRANSPILER_H

#define SIMULATOR_TRANSPILER_H

#include "json.hpp"
#include <string>
#include <list>
#include <utility>
#include "parser.hh"

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
        std::map<std::string,std::vector<unsigned int>> valid_modules = {{"and_module",{1,1,1}},{"master_clk",{1}}};
        std::map<std::string,std::vector<unsigned int>> valid_inputs  = {{"button",{1}}};
        std::map<std::string,std::vector<unsigned int>> valid_outputs = {{"led",{1}}};

        std::set<std::string> valid_configs = {"sim_frequency_min", "sim_frequency_max", "frame_rate_cap"};

        void setup_dbs();
        void graph_analysis();
        static std::pair<std::string,unsigned int> resolve_access(nlohmann::json &dbs, nlohmann::json &lookup);

        class node{
        public:
            std::string name;
            std::string n_type;
            std::list<node*> incoming;
            std::list<node*> outgoing;
            int level;
            bool visited = false;
            node(std::string name, std::string n_type){
                this->name = std::move(name);
                this->n_type = std::move(n_type);
            };
        };
    public:
        yy::location location;
        static nlohmann::json transpile(const std::string&);
        friend class yy::parser;
    };
}
#endif