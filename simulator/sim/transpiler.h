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
            enum token_type {NAME, SYSTEM_COMMAND, DECL_END, ARGS_START, ARGS_END, LIST_DELIMITER, ACCESSOR, WIRE, BYTE, WORD, AND_MODULE};
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
    public:
        static nlohmann::json transpile(const std::string& path);
    };

} // sim

#endif //SIMULATOR_TRANSPILER_H
