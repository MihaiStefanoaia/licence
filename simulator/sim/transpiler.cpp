//
// Created by mihai-laptop on 4/4/23.
//

#include <fstream>
#include <regex>
#include <iostream>
#include <locale>
#include <set>
#include "transpiler.h"
#include <stdexcept>

namespace sim {
    std::stringstream transpiler::preprocess_file(const std::string& path) {

        std::ifstream file(path);

        std::string processed(std::istreambuf_iterator<char>(file),{});

        std::cout << processed << '\n';
        processed = std::regex_replace(processed,std::regex("<<-.*->>")," ");

        processed = std::regex_replace(processed,std::regex(";")," ; ");
        processed = std::regex_replace(processed,std::regex("=")," = ");
        processed = std::regex_replace(processed,std::regex("\\.")," . ");
        processed = std::regex_replace(processed,std::regex(",")," , ");

        processed = std::regex_replace(processed,std::regex("\\(")," ( ");
        processed = std::regex_replace(processed,std::regex("\\)")," ) ");

        processed = std::regex_replace(processed,std::regex("\\[")," [ ");
        processed = std::regex_replace(processed,std::regex("\\]")," ] ");

        std::cout << processed << '\n';
        std::stringstream ret(processed);
        return ret;
    }



    nlohmann::json transpiler::transpile(const std::string &path) {
        auto preprocessed = preprocess_file(path);
        auto tokens = tokenize(preprocessed);
        auto ast = generate_ast(tokens);
        return {};
    }

    std::list<transpiler::token> transpiler::tokenize(std::stringstream &ss) {
        std::set<std::string> wires = {"wire", "byte", "word"};
        std::set<std::string> modules = {"and_module"};
        std::map<std::string,token::token_type> basic_tokens = {{"(",token::ARGS_BEGIN},
                                                      {")",token::ARGS_END},
                                                      {";",token::DECL_END},
                                                      {",",token::LIST_DELIMITER},
                                                      {".",token::ACCESSOR},
                                                      {"[",token::ARRAY_ACCESSOR_BEGIN},
                                                      {"]",token::ARRAY_ACCESSOR_END}};
        std::list<transpiler::token> ret = {};
        std::string str;
        while(ss >> str){
            std::cout << str << '\n';
            if(basic_tokens.count(str)) {
                ret.emplace_back(basic_tokens[str], str);
            } else if(wires.count(str)) {
                ret.emplace_back(token::WIRE_DECL, str);
            } else if(modules.count(str)) {
                ret.emplace_back(token::MODULE_DECL, str);
            } else if(std::regex_match(str,std::regex("\\$.*"))){
                ret.emplace_back(token::SYSTEM_COMMAND, str);
            } else if(std::regex_match(str,std::regex("^(0|[1-9][0-9]*)$"))) {
                ret.emplace_back(token::NUMBER, str);
            } else if(std::regex_match(str,std::regex(R"(^[_a-z][_a-z0-9]*$)"))) {
                ret.emplace_back(token::NAME, str);
            } else {
                throw std::runtime_error("invalid token \"" + str + "\"");
            }
        }
        return ret;
    }

    std::list<transpiler::declaration> transpiler::generate_ast(std::list<token> &) {
        return {};
    }
} // sim