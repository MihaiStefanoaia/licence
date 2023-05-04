#include <set>
#include "transpiler.h"
namespace sim{
    transpiler::transpiler() : trace_scanning(false), trace_parsing(false){
    }

    int transpiler::parse(const std::string& str){
        file = str;
        location.initialize (&file);
        scan_begin ();
        yy::parser parse (*this);
        parse.set_debug_level (trace_parsing);
        int res = parse ();
        scan_end ();
        return res;
    }


    nlohmann::json transpiler::transpile(const std::string &path) {
        transpiler instance;
        int parse_res = instance.parse(path);
        if(parse_res){
            std::string str = "Parser exited with status " + std::to_string(parse_res);
            throw std::runtime_error(str);
        }
        instance.setup_dbs();
        return instance.ret;
    }

    void transpiler::setup_dbs() {
        std::set<std::string> wire_identifiers = {};
        std::map<std::string,unsigned int> valid_modules = {{"and_module",3},{"master_clk",1}};
        std::map<std::string,unsigned int> valid_inputs  = {{"button",1}};
        std::map<std::string,unsigned int> valid_outputs = {{"led",1}};
        std::set<std::string> valid_configs = {"sim_frequency_min", "sim_frequency_max", "frame_rate_cap"};
        nlohmann::json fin;
        std::string err;
        fin["wire_db"] = nlohmann::json::array();
        fin["component_db"] = nlohmann::json::array();
        fin["io_db"]["inputs"] = nlohmann::json::array();
        fin["io_db"]["outputs"] = nlohmann::json::array();

        for(auto& stmt : ret){
            if(!stmt.contains("stmt_type")){
                err = "No statement type. How?";
                throw std::runtime_error(err);
            }
            if(stmt["stmt_type"] != "sys_cmd"){
                if(wire_identifiers.count(stmt["name"]))
                    throw std::runtime_error("Identifier \"" + std::string(stmt["name"]) + "\" is defined twice.");
            }
            if(stmt["stmt_type"] == "sys_cmd"){
                fin["config_db"][stmt["type"]] = stmt["value"];
            } else if(stmt["stmt_type"] == "wire_decl"){
                std::string w_type = stmt["type"];
                if(stmt["type"] != "wire")
                    throw std::runtime_error("Invalid type identifier " + w_type);
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                fin["wire_db"] += tmp;
                wire_identifiers.insert(stmt["name"]);
            } else if(stmt["stmt_type"] == "module_decl"){
                if(!valid_modules.count(stmt["type"]) && !valid_inputs.count(stmt["type"]) && !valid_outputs.count(stmt["type"]))
                    throw std::runtime_error("Invalid type identifier " + std::string(stmt["type"]));
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                tmp["type"] = stmt["type"];
                tmp["args"] = nlohmann::json::array();
                for(auto arg : stmt["args"]){
                    if(arg["type"] == "basic"){
                        if(!wire_identifiers.count(arg["name"]))
                            throw std::runtime_error("Identifier \"" + std::string(arg["name"]) + "\" is not defined");
                        tmp["args"] += arg["name"];
                    } else {
                        throw std::runtime_error("Unsupported feature (yet)");
                    }
                }
                size_t argc = tmp["args"].size();
                if( (valid_inputs .count(tmp["type"]) && argc != valid_inputs [tmp["type"]]) ||
                    (valid_modules.count(tmp["type"]) && argc != valid_modules[tmp["type"]]) ||
                    (valid_outputs.count(tmp["type"]) && argc != valid_outputs[tmp["type"]])){
                    unsigned int expected = 0;
                    if(valid_inputs.count(tmp["type"])){
                        expected = valid_inputs[tmp["type"]];
                    } else if(valid_modules.count(tmp["type"])){
                        expected = valid_modules[tmp["type"]];
                    } else {
                        expected = valid_outputs[tmp["type"]];
                    }
                    err = "incorrect amount of arguments in generation of module ";
                    err += std::string(tmp["type"]);
                    err += " (expected " + std::to_string(expected);
                    err += ", got " + std::to_string(argc) + ")";
                    throw std::runtime_error(err);
                }

                if(tmp["type"] == "master_clk"){
                    if(fin["config_db"].contains("master_clk"))
                        throw std::runtime_error("Only one instance of master_clk is allowed");
//                    throw std::runtime_error("not supported yet....");
                    fin["config_db"]["reactive_only"] = 0;
                    fin["config_db"]["master_clk"] = tmp["args"][0];
                } else if(valid_modules.count(tmp["type"]))
                    fin["component_db"] += tmp;
                else if(valid_inputs.count(tmp["type"]))
                    fin["io_db"]["inputs"] += tmp;
                else
                    fin["io_db"]["outputs"] += tmp;

            } else {
                err = "Invalid type \"";
                err += stmt["type"];
                err += "\". How?";
                throw std::runtime_error(err);
            }
        }
        ret = fin;
        std::cout << "dbs setup:\n";
        std::cout << ret.dump(2);
    }

}