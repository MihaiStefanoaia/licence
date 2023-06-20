#include <set>
#include <queue>
#include <stack>
#include "transpiler.h"
#include "level_promise.h"

namespace sim{
    class transpiler::node{
    public:
        enum drive_type {NONE, NEGATIVE, POSITIVE, BOTH, REACTIVE};

        std::string name;
        std::string n_type;
        drive_type driven = REACTIVE;
        std::set<node*> incoming;
        std::set<node*> outgoing;
        level_promise level = level_promise(level_promise::NONE,{});

        std::vector<std::string> arguments;


        bool visited = false;
        node(std::string name, std::string n_type){
            this->name = std::move(name);
            this->n_type = std::move(n_type);
        };
        void drive(drive_type on_what) {
            driven = on_what;
        }
        void connect_incoming(node* other){
            if(n_type == "wire" && !(incoming.empty() || incoming.count(other)))
                throw std::runtime_error("wires cannot be driven by more than one source");
            incoming.insert(other);
            other->outgoing.insert(this);
        }

        void connect_outgoing(node* other){
            if(other->n_type == "wire" && !(other->incoming.empty() || other->incoming.count(this)))
                throw std::runtime_error("wires cannot be driven by more than one source");
            outgoing.insert(other);
            other->incoming.insert(this);
        }

        bool is_prunable(node* other) const{
            if( (driven == NEGATIVE && other->driven == POSITIVE) ||
                (driven == POSITIVE && other->driven == NEGATIVE) ||
                other->driven == NONE)
                return true;
            return false;
        }

        void prune(node* other){
            if(incoming.count(other)){
                incoming.erase(other);
                other->outgoing.erase(this);
            }
            if(outgoing.count(other)){
                outgoing.erase(other);
                other->incoming.erase(this);
            }
        }

        bool make_unreactive(){
            if(driven != REACTIVE)
                return false;

            auto new_drive = NONE;
            for(auto inc : incoming){
                if(inc->driven == REACTIVE)
                    return false;
                if(inc->driven == NONE)
                    continue;
                if((new_drive == POSITIVE && inc->driven == NEGATIVE) ||
                   (new_drive == NEGATIVE && inc->driven == POSITIVE) ||
                   new_drive == BOTH || inc->driven == BOTH)
                    new_drive = BOTH;
                else
                    new_drive = inc->driven;
            }
            driven = new_drive;
            return true;
        }

        friend std::ostream& operator<<(std::ostream& o, node& n){
            o << "Node:\n";
            o << "Name: " << n.name << '\n';
            o << "Type: " << n.n_type << '\n';
            o << "Driven on: ";
            switch (n.driven) {

                case NONE:
                    o << "none\n";
                    break;
                case NEGATIVE:
                    o << "negative\n";
                    break;
                case POSITIVE:
                    o << "positive\n";
                    break;
                case BOTH:
                    o << "both\n";
                    break;
                case REACTIVE:
                    o << "reactive\n";
                    break;
            }
            o << "Arguments: [";
            for(const auto& arg : n.arguments){
                o << arg << ' ';
            }
            o << "]\n";
            o << "Incoming connections: [";
            for(const auto& inc : n.incoming){
                o << inc->name << ' ';
            }
            o << "]\n";
            o << "Outgoing connections: [";
            for(const auto& otg : n.outgoing){
                o << otg->name << ' ';
            }
            o << "]\n";
            return o;
        }
    };

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
        instance.transpiler_init();
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
        std::set<std::string> identifiers = {"nil", "sim_monitor"};

        nlohmann::json fin;
        std::string err;
        fin["wire_db"] = nlohmann::json::array();
        fin["array_db"] = nlohmann::json::array();
        fin["component_db"] = nlohmann::json::array();
        fin["io_db"]["inputs"] = nlohmann::json::array();
        fin["io_db"]["outputs"] = nlohmann::json::array();
        fin["window_db"] = nlohmann::json::array();

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

            } else if(stmt["stmt_type"] == "layout_decl"){
                nlohmann::json tmp;
                tmp["name"] = stmt["name"];
                tmp["args"] = stmt["args"];
                for(auto arg : stmt["args"]){
                    if(!is_monitored(fin,arg["content"])){
                        throw std::runtime_error("argument " + std::string(arg["content"]) + " is not a monitored element");
                    }
                }
                fin["window_db"] += tmp;
            } else {
                err = "Invalid type \"";
                err += stmt["type"];
                err += "\". How?";
                throw std::runtime_error(err);
            }
        }
        ret = fin;
        std::cout << "dbs setup:\n";
        std::cout << ret.dump(2) << "\n";
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
        std::cout << "running graph analysis:\n";

        //build all nodes
        std::map<std::string, node*> nodes;

        std::cout << "building wires....\n";
        //wires
        for(auto wire : ret["wire_db"]){
            nodes[wire["name"]] = new node(wire["name"],"wire");
            nodes[wire["name"]]->driven = node::REACTIVE;
        }

        std::cout << "building components....\n";
        //components
        for(auto component : ret["component_db"]){
            auto tmp = new node(component["name"], component["type"]);
            nodes[component["name"]] = tmp;
            for(const auto& arg : component["args"])
                tmp->arguments.push_back(arg);
            if(positive_driven.count(component["type"])){
                tmp->drive(node::POSITIVE);
            } else if(negative_driven.count(component["type"])){
                tmp->drive(node::NEGATIVE);
            } else if(always_driven.count(component["type"])){
                tmp->drive(node::BOTH);
            } else if(reactive_driven.count(component["type"])){
                tmp->drive(node::REACTIVE);
            }
            tmp->level.op = level_promise::MAX_INC;
        }

        std::cout << "building inputs....\n";
        //input components
        for(auto input : ret["io_db"]["inputs"]){
            auto tmp = new node(input["name"],input["type"]);
            nodes[input["name"]] = tmp;
            tmp->level.op = level_promise::BASE;
            tmp->drive(node::NEGATIVE);
            for(const auto& arg : input["args"])
                tmp->arguments.push_back(arg);
        }

        std::cout << "setting up the master clock....\n";
        //master clk, if available
        if(ret["config_db"].contains("master_clk")) {
            nodes[ret["config_db"]["master_clk"]]->level.op = level_promise::BASE;
            nodes[ret["config_db"]["master_clk"]]->drive(node::BOTH);
        }

        std::cout << "connecting the nodes....\n";
        //set up the graph connections
        for(const auto& kvp : nodes){
            auto _node = kvp.second;
            // take each argument, and each wire in it if it is the case
            auto &read = read_args[_node->n_type];
            for(unsigned int i = 0; i < read.size();i++) {
                auto &arg = _node->arguments[i];
                auto arg_wires = get_wires_from_argument(ret, arg);
                for(const auto& arg_wire : arg_wires) {
                    if(read[i]){ // if the argument is read
                        _node->connect_incoming(nodes[arg_wire]);
                    } else {
                        _node->connect_outgoing(nodes[arg_wire]);
                    }
                }
            }
        }
        try {
            std::cout << "pruning connections....\n";
            prune_connections(nodes);

            std::cout << "\n\n\n\nTHE FULL RESULT OF THE GRAPH PRUNING:\n\n";
            for (const auto &n: nodes) {
                std::cout << *(n.second) << '\n';
            }

            std::cout << "finding cycles in the pruned graph....\n";
            detect_cycles(nodes);

            std::cout << "determining levels....\n";
            determine_levels(nodes);


            std::cout << "\n\n\nALL OF THE LEVELS:\n\n";
            for (const auto &n: nodes) {
                std::cout << n.second->name << ": " << n.second->level.value << '\n';
            }
        } catch(std::runtime_error& error) {
            graph_cleanup(nodes);
            throw error;
        }

        for(const auto& kvp : nodes){
            auto node_ = kvp.second;
            if(node_->n_type == "wire"){
                for(auto& wire : ret["wire_db"]){
                    if(wire["name"] == node_->name){
                        wire["level"] = node_->level.value;
                    }
                }
            } else {
                for(auto& component : ret["component_db"]){
                    if(component["name"] == node_->name){
                        component["level"] = node_->level.value;
                    }
                }
            }
        }

        graph_cleanup(nodes);
    }

    std::list<std::string> transpiler::get_wires_from_argument(nlohmann::json& dbs, const std::string& lookup) {
        for(auto wire : dbs["wire_db"]){
            if(lookup == wire["name"])
                return {lookup};
        }
        for(auto array : dbs["array_db"]){
            if(lookup == array["name"]){
                std::list<std::string> ret;
                for(const auto& arg : array["args"])
                    ret.emplace_back(arg);
                return ret;
            }
        }
        throw std::runtime_error("Invalid lookup for graph analysis. No wire or array called \"" + lookup + "\"");
    }

    void transpiler::prune_connections(std::map<std::string,node *> &graph) {
        bool change = true;
        while(change){
            change = false;
            for(const auto& kvp : graph){
                auto _node = kvp.second;
                change = _node->make_unreactive();
                std::list<node*> marked;
                for(const auto& inc : _node->incoming){
                    if(_node->is_prunable(inc)){
                        change = true;
                        marked.push_back(inc);
                    }
                }
                for(auto m : marked)
                    _node->prune(m);
            }
        }
    }

    void transpiler::process_dfs_tree (std::map<node*,int> &visited, std::stack<node*> &stack){
        auto check = stack.top();
        auto print_stack = [](std::stack<node*> stack_){
            std::string circle = stack_.top()->name;
            stack_.pop();
            while(!stack_.empty()){
                circle = stack_.top()->name + " -> " + circle;
                stack_.pop();
            }
            return circle;
        };
        std::cout << print_stack(stack) << "\n";
        for(auto adj : check->outgoing){
            if(visited[adj] == 1){
                std::string err = "Found circular dependency:\n";
                err += check->name + print_stack(stack);
                throw std::runtime_error(err);
            } else if(visited[adj] == 0) {
                stack.push(adj);
                visited[adj] = 1;
                process_dfs_tree(visited,stack);
            }
        }
        visited[check] = 2;
        stack.pop();
    }

    void transpiler::detect_cycles(std::map<std::string, node *> &graph) {
        // 0 - not visited
        // 1 - visited, in stack
        // 2 - visited, done
        std::map<node*,int> visited;
        std::stack<node*> stack;

        for(const auto& kvp : graph){
            visited[kvp.second] = 0;
        }
        for(auto kvp : visited){
            if(kvp.second != 0)
                continue;
            stack.push(kvp.first);
            visited[kvp.first] = 1;
            process_dfs_tree(visited,stack);
        }
    }

    void transpiler::determine_levels(std::map<std::string, node *> &graph) {
        for(const auto& kvp : graph){
            auto _node = kvp.second;
            if(_node->driven == node::NONE || _node->incoming.empty()){
                _node->level.op = level_promise::BASE;
                continue;
            }
            for(auto dpd : _node->incoming){
                _node->level.add_dependency(&dpd->level);
                if(_node->n_type == "wire")
                    _node->level.op = level_promise::INCREMENT;
                else
                    _node->level.op = level_promise::MAX_INC;
            }
        }
        bool all_determined = false;
        while(!all_determined){
            all_determined = true;
            std::cout << "determining...";
            for(const auto& kvp : graph){
                auto& lvl = kvp.second->level;
                lvl.determine();
                all_determined &= lvl.determined;
            }
        }
    }

    void transpiler::graph_cleanup(const std::map<std::string, node *>& graph) {
        for(const auto& kvp : graph)
            delete kvp.second;
    }

    bool transpiler::is_monitored(nlohmann::json &dbs, const std::string &lookup) {
        if(lookup == "sim_monitor"){
            return true;
        }
        for(auto window : dbs["window_db"]){
            if(window["name"] == lookup)
                return true;
        }

        for(auto input : dbs["io_db"]["inputs"]){
            if(input["name"] == lookup)
                return true;
        }
        for(auto output : dbs["io_db"]["outputs"]){
            if(output["name"] == lookup)
                return true;
        }

        for(auto md : dbs["component_db"]){
            if(md["name"] == lookup && monitored.count(md["type"]))
                return true;
        }
        return false;
    }

}