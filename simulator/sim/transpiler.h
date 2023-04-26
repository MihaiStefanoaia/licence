#ifndef SIMULATOR_TRANSPILER_H

#define SIMULATOR_TRANSPILER_H

#include "json.hpp"
#include <string>
#include "parser.hh"

// Give Flex the prototype of yylex we want ...
# define YY_DECL yy::parser::symbol_type yylex (sim::transpiler& trp)
// ... and declare it for the parser's sake.
YY_DECL;
namespace sim{
    class transpiler{
    public:
        transpiler();
        std::string file;
        nlohmann::json ret;
        int parse(const std::string& str);
        int result;
        void scan_begin ();
        void scan_end ();
        bool trace_scanning;
        bool trace_parsing;
        yy::location location;
        int add_semantics();
        static nlohmann::json transpile(const std::string&);
    };
}
#endif