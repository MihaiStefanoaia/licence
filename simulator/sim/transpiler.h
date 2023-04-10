//
// Created by mihai-laptop on 4/4/23.
//

#ifndef SIMULATOR_TRANSPILER_H
#define SIMULATOR_TRANSPILER_H


#include <sstream>
#include <string>
#include <list>
#include "../json.hpp"

namespace sim {

    class transpiler {
        class token{
        public:
            enum token_type {NAME, NUMBER, SYSTEM_COMMAND, DECL_END, ARGS_BEGIN, ARGS_END, ACCESSOR, ARRAY_ACCESSOR_BEGIN, ARRAY_ACCESSOR_END, LIST_DELIMITER, WIRE_DECL, MODULE_DECL};
            std::string value;
            token_type t_type;
            token(token_type t_type,std::string& value){
                this->t_type = t_type;
                this->value = value;
            }
        };
        class declaration{
        public:
            std::string module_type;
            std::string instance_name;
            std::list<std::string> arguments = {};
        };
        static std::stringstream preprocess_file(const std::string& path);
        static std::list<token> tokenize(std::stringstream&);
        static std::list<declaration> generate_ast(std::list<token>&);
    public:
        static nlohmann::json transpile(const std::string& path);
    };

} // sim

#endif //SIMULATOR_TRANSPILER_H