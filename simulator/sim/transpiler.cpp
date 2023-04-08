//
// Created by mihai-laptop on 4/4/23.
//

#include <fstream>
#include <regex>
#include <iostream>
#include <locale>
#include "transpiler.h"

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
        std::stringstream preprocessed = preprocess_file(path);
        std::list<token> tokens = tokenize(preprocessed);
        return {};
    }

    std::list<transpiler::token> transpiler::tokenize(std::stringstream &ss) {
        std::list<transpiler::token> ret = {};
        std::string str;
        while(ss >> str){
            std::cout << str << '\n';
            if(std::regex_match(str,std::regex("\\$.*"))){
                ret.emplace_back(token::SYSTEM_COMMAND, str);
            } else if(str == "(") {
                ret.emplace_back(token::ARGS_START, str);
            } else if(str == ")") {
                ret.emplace_back(token::ARGS_END, str);
            } else if(str == ";") {
                ret.emplace_back(token::DECL_END, str);
            } else if(str == ",") {
                ret.emplace_back(token::LIST_DELIMITER, str);
            } else if(str == "wire") {
                ret.emplace_back(token::WIRE, str);
            } else if(str == "byte") {
                ret.emplace_back(token::BYTE, str);
            } else if(str == "word") {
                ret.emplace_back(token::WORD, str);
            } else if(str == "and_module") {
                ret.emplace_back(token::AND_MODULE, str);
            } else {
                ret.emplace_back(token::NAME, str);
            }
        }
        return ret;
    }
} // sim