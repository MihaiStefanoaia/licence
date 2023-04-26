#include "transpiler.h"
namespace sim{
    transpiler::transpiler() : trace_parsing(false), trace_scanning(false){
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
        instance.add_semantics();
        return instance.ret;
    }

    int transpiler::add_semantics() {
        nlohmann::json fin;
        std::string err;
        fin["config_db"] = nlohmann::json::array();
        fin["wire_db"] = nlohmann::json::array();
        fin["module_db"] = nlohmann::json::array();
        for(auto& stmt : ret){
            if(stmt["stmt_type"] == "sys_cmd"){
                nlohmann::json tmp;
                tmp["cmd"] = stmt["type"];
                tmp["val"] = stmt["value"];
                fin["config_db"] += tmp;
            } else {
                err = "Invalid type ";
                err += stmt["type"];
                err += ". How?";
                throw std::runtime_error(err);
            }
        }
        ret = fin;
        return 0;
    }

}