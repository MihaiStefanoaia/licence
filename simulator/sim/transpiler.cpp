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

//        std::cout << processed << '\n';
        processed = std::regex_replace(processed,std::regex("<<-.*->>")," ");

        processed = std::regex_replace(processed,std::regex(";")," ; ");
        processed = std::regex_replace(processed,std::regex("=")," = ");
        processed = std::regex_replace(processed,std::regex("\\.")," . ");
        processed = std::regex_replace(processed,std::regex(",")," , ");

        processed = std::regex_replace(processed,std::regex("\\(")," ( ");
        processed = std::regex_replace(processed,std::regex("\\)")," ) ");

        processed = std::regex_replace(processed,std::regex("\\[")," [ ");
        processed = std::regex_replace(processed,std::regex("\\]")," ] ");

//        std::cout << processed << '\n';
        std::stringstream ret(processed);
        return ret;
    }



    nlohmann::json transpiler::transpile(const std::string &path) {
        auto preprocessed = preprocess_file(path);
        auto tokens = tokenize(preprocessed);
        auto ast = generate_ast(tokens);
        auto ret = add_semantics(ast);
        return ret;
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
            } else if(str == "true" || str == "false" || std::regex_match(str,std::regex("^(0|[1-9][0-9]*)$"))) {
                ret.emplace_back(token::VALUE, str);
            } else if(std::regex_match(str,std::regex(R"(^[_a-z][_a-z0-9]*$)"))) {
                ret.emplace_back(token::NAME, str);
            } else {
                throw std::runtime_error("invalid token \"" + str + "\"");
            }
        }
        str = "";
        ret.emplace_back(token::NAME, str);
        return ret;
    }

    nlohmann::json transpiler::generate_ast(std::list<token> &tokens) {
        nlohmann::json ret;
        ret["document"] = nlohmann::json::array();
        int nr_stmt = 0;
        std::cout << ret.dump(2);
        std::map<std::pair<std::string,token::token_type>,std::pair<std::string,int>> valid_transitions =
        {
            {{"document",token::SYSTEM_COMMAND},{"cmd",0}},
            {{"document",token::WIRE_DECL},{"decl",0}},
            {{"document",token::MODULE_DECL},{"decl",0}},
            {{"cmd",token::ARGS_BEGIN},{"cmd_args",0}},
            {{"cmd_args",token::VALUE},{"cmd_value",0}},
            {{"cmd_value",token::ARGS_END},{"__pop",2}},
            {{"stmt",token::DECL_END},{"__pop",1}}
        };
        std::stack<std::string> context_stack;
        context_stack.emplace("document");
        token::token_type last_token;
        for(auto& tk : tokens){
            std::string con = context_stack.top();
            if(valid_transitions.count({con,tk.t_type})){
                auto& transition = valid_transitions[{con,tk.t_type}];

                if(transition.first == "__pop"){
                    for(int i = 0; i < transition.second; i++)
                        context_stack.pop();
                } else if(transition.first != "__continue"){
                    context_stack.push(transition.first);
                }
                if(tk.t_type == token::DECL_END){
                    nr_stmt++;
                }
            } else {
                throw std::runtime_error("invalid token \"" + tk.value + "\"");
            }
        }

        return ret;
    }

    nlohmann::json transpiler::add_semantics(nlohmann::json& ast) {
        return false;
    }
} // sim