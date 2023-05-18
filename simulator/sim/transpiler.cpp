#include <set>
#include "transpiler.h"
namespace sim{
    transpiler::transpiler() : trace_scanning(false), trace_parsing(false){
        result = 0;
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
        instance.graph_analysis();
        return instance.ret;
    }

    void transpiler::setup_dbs() {
        std::set<std::string> identifiers = {"nil"};

        nlohmann::json fin;
        std::string err;
        fin["wire_db"] = nlohmann::json::array();
        fin["array_db"] = nlohmann::json::array();
        fin["component_db"] = nlohmann::json::array();
        fin["io_db"]["inputs"] = nlohmann::json::array();
        fin["io_db"]["outputs"] = nlohmann::json::array();

        for(auto& stmt : ret){
            if(!stmt.contains("stmt_type")){
                err = "No statement type. How?";
                throw std::runtime_error(err);
            }
            //if it is not a configuration setting, check if an object with the identifier has not been created yet
            //if it is a new identifier, add it to the list of reserved identifiers
            if(stmt["stmt_type"] != "sys_cmd"){
                if(identifiers.count(stmt["name"]))
                    throw std::runtime_error("Identifier \"" + std::string(stmt["name"]) + "\" is defined multiple times");
                identifiers.insert(stmt["name"]);
            }

            if(stmt["stmt_type"] == "sys_cmd"){ //set the config to the inputted value. straightforward
                fin["config_db"][stmt["type"]] = stmt["value"];
            } else if(stmt["stmt_type"] == "wire_decl"){ //simply add the wire to the list of wires
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                fin["wire_db"] += tmp;
            } else if(stmt["stmt_type"] == "autogen_decl"){ //generate a sized array and the wires inside it
                std::cout << "auto generating the wires......\n";
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                tmp["size"] = stmt["size"];
                tmp["args"] = nlohmann::json::array();
                for(int i = 0; i < tmp["size"]; i++){
                    nlohmann::json wire_tmp;
                    wire_tmp["name"] = (std::string(tmp["name"]) + "_" + std::to_string(i));
                    if(identifiers.count(wire_tmp["name"])){
                        throw std::runtime_error("wire " + std::string(wire_tmp["name"]) + " has already been defined");
                    } else {
                        fin["wire_db"] += wire_tmp;
                        identifiers.insert(wire_tmp["name"]);
                    }
                    tmp["args"] += wire_tmp["name"];
                }
                fin["array_db"] += tmp;

            } else if(stmt["stmt_type"] == "array_decl"){ //generate a sized array
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                tmp["size"] = stmt["size"];
                if(tmp["size"] < 2)
                    throw std::runtime_error("an array cannot be smaller than 2 elements");
                tmp["args"] = nlohmann::json::array();
                for(auto arg : stmt["args"]){
                    auto info = resolve_access(fin,arg);
                    if(info.second != 1)
                        throw std::runtime_error("argument with invalid length " + std::to_string(info.second) + " in generating array " + std::string(arg["name"]));
                    tmp["args"] += info.first;
                }
                // allow for incomplete definition by padding the end with nil
                // for example the statement:
                // array<2> arr(arg);
                // is the same as:
                // array<2> arr(arg,nil);
                // nil is just a non-writeable wire
                // any writes to it will not change it and any writes will return false

                // the generation downstream of this also allows to have too many arguments to a sized array
                // but the extra ones are just ignored

                while(tmp["args"].size() < tmp["size"]){
                    tmp["args"] += "nil";
                }

                fin["array_db"] += tmp;

            } else if(stmt["stmt_type"] == "module_decl"){ //generate a module (including I/O modules)
                if( !valid_modules.count(stmt["type"]) &&
                    !valid_inputs .count(stmt["type"]) &&
                    !valid_outputs.count(stmt["type"])) //check for a valid type
                    throw std::runtime_error("Invalid type identifier \"" + std::string(stmt["type"]) + "\"");
                nlohmann::json tmp;
                std::vector<unsigned int> arg_sizes;

                tmp["name"] = stmt["name"];
                tmp["type"] = stmt["type"];
                tmp["args"] = nlohmann::json::array();

                if(valid_inputs.count(tmp["type"])){
                    arg_sizes = valid_inputs[tmp["type"]];
                } else if(valid_modules.count(tmp["type"])){
                    arg_sizes = valid_modules[tmp["type"]];
                } else {
                    arg_sizes = valid_outputs[tmp["type"]];
                }

                // check for correct amount of arguments, this time it is important
                if(stmt["args"].size() != arg_sizes.size()){
                    unsigned int expected = 0;
                    if(valid_inputs.count(tmp["type"])){
                        expected = valid_inputs[tmp["type"]].size();
                    } else if(valid_modules.count(tmp["type"])){
                        expected = valid_modules[tmp["type"]].size();
                    } else {
                        expected = valid_outputs[tmp["type"]].size();
                    }
                    err = "incorrect amount of arguments in generation of module ";
                    err += std::string(tmp["type"]);
                    err += " (expected " + std::to_string(expected);
                    err += ", got " + std::to_string(arg_sizes.size()) + ")";
                    throw std::runtime_error(err);
                }

                int i = 0;
                for(auto arg : stmt["args"]){
                    auto info = resolve_access(fin,arg);
                    if(info.second != arg_sizes[i])
                        throw std::runtime_error("incorrect argument size");
                    tmp["args"] += info.first;
                    i++;
                }

                // check if it is an instance of master_clk and if it is, check if there is another
                // otherwise just add it to the appropriate database
                if(tmp["type"] == "master_clk"){
                    if(fin["config_db"].contains("master_clk"))
                        throw std::runtime_error("only one instance of master_clk is allowed");
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

    std::pair<std::string,unsigned int> transpiler::resolve_access(nlohmann::json &dbs, nlohmann::json &lookup) {
        if(lookup["type"] == "basic") {
            if(lookup["name"] == "nil")
                return {"nil",1};

            for(auto& wire : dbs["wire_db"])
                if(lookup["name"] == wire["name"])
                    return {lookup["name"],1};

            for(auto& array : dbs["array_db"])
                if(lookup["name"] == array["name"])
                    return {lookup["name"],array["size"]};

        } else if(lookup["type"] == "array_access"){
            for(auto& arr : dbs["array_db"]){
                if(lookup["name"] == arr["name"]) {
                    int i = lookup["index"];
                    if (i < arr["size"])
                        return {arr["args"][i], 1};
                    else
                        throw std::runtime_error("array access out of bounds (" + std::to_string(i) + " with maximum of " + std::string(arr["size"]) + ")");
                }
            }
        } else {
            throw std::runtime_error("invalid or unsupported access type");
        }
        throw std::runtime_error("identifier \"" + std::string(lookup["name"]) + "\" does not exist");
    }

    void transpiler::graph_analysis() {
        std::map<std::string,node*> elements;
        for(auto& wire : ret["wire_db"]){
            elements[wire["name"]] = new node(wire["name"], "wire");
        }
        for(auto& input : ret["io_db"]["inputs"]){
            elements[input["name"]] = new node(input["name"],input["type"]);
            if(input["type"] == "button"){
                for(auto& arg : input["args"]){
                }
            }
        }

        for(auto& component : ret["component_db"]){
        }
    }

}