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
            std::string str = "Parser exited with status " + std::to_string(parse_res) + ", loading example";
            throw std::runtime_error(str);
        }
        instance.setup_dbs();
        return instance.ret;
    }

    int transpiler::setup_dbs() {
        std::set<std::string> identifiers = {};
        std::set<std::string> valid_modules = {"and_module", "mux_2x1"};
        nlohmann::json fin;
        std::string err;
        fin["config_db"] = nlohmann::json::array();
        fin["wire_db"] = nlohmann::json::array();
        fin["component_db"] = nlohmann::json::array();
        for(auto& stmt : ret){
            if(!stmt.contains("stmt_type")){
                err = "No type. How?";
                throw std::runtime_error(err);
            }
            if(stmt["stmt_type"] == "sys_cmd"){
                nlohmann::json tmp;
                tmp["cmd"] = stmt["type"];
                tmp["val"] = stmt["value"];
                fin["config_db"] += tmp;
            } else if(stmt["stmt_type"] == "wire_decl"){
                std::string w_type = stmt["type"];
                if(stmt["type"] != "wire")
                    throw std::runtime_error("Invalid type identifier " + w_type);
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                fin["wire_db"] += tmp;
            } else if(stmt["stmt_type"] == "module_decl"){
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                tmp["type"] = stmt["type"];
                tmp["args"] = nlohmann::json::array();
                for(auto arg : stmt["args"]){
                    if(arg["type"] != "basic")
                        throw std::runtime_error("Unsupported feature (yet)");
                    tmp["args"] += arg["name"];
                }
                fin["component_db"] += tmp;

            } else {
                err = "Invalid type \"";
                err += stmt["type"];
                err += "\". How?";
                throw std::runtime_error(err);
            }
        }
        ret = fin;
        return 0;
    }

}