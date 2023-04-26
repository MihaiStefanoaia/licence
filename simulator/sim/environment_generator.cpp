//
// Created by mihai-pc on 3/31/23.
//

#include <fstream>
#include <iostream>
#include "environment_generator.h"
#include "transpiler.h"

namespace sim {

    nlohmann::json environment_generator::generate(const std::string& path) {
        if(path == "example")
            return example();
        try{
            return sim::transpiler::transpile(path);
        } catch(std::runtime_error err) {
            std::cout << err.what() << "\n";
            return example();
        }
    }

    nlohmann::json environment_generator::example() { //((a&b)&c) - pre-validated
        using namespace nlohmann;
        json ret;

        ret["has_master_clock"] = false;

        ret["wire_db"][0]["name"] = "wire_a";
        ret["wire_db"][0]["level"] = 0;

        ret["wire_db"][1]["name"] = "wire_b";
        ret["wire_db"][1]["level"] = 0;

        ret["wire_db"][2]["name"] = "wire_c";
        ret["wire_db"][2]["level"] = 0;

        ret["wire_db"][3]["name"] = "output0";
        ret["wire_db"][3]["level"] = 2;

        ret["wire_db"][4]["name"] = "output1";
        ret["wire_db"][4]["level"] = 4;


        ret["component_db"][0]["type"] = "and_module";
        ret["component_db"][0]["name"] = "and_mod0";
        ret["component_db"][0]["level"] = 1;
        ret["component_db"][0]["args"] = {"wire_a", "wire_b", "output0"};

        ret["component_db"][1]["type"] = "and_module";
        ret["component_db"][1]["name"] = "and_mod1";
        ret["component_db"][1]["level"] = 3;
        ret["component_db"][1]["args"] = {"output0", "wire_c", "output1"};


        ret["io_db"]["inputs"][0]["type"] = "button";
        ret["io_db"]["inputs"][0]["name"] = "a";
        ret["io_db"]["inputs"][0]["args"] = {"wire_a"};

        ret["io_db"]["inputs"][1]["type"] = "button";
        ret["io_db"]["inputs"][1]["name"] = "b";
        ret["io_db"]["inputs"][1]["args"] = {"wire_b"};

        ret["io_db"]["inputs"][2]["type"] = "button";
        ret["io_db"]["inputs"][2]["name"] = "c";
        ret["io_db"]["inputs"][2]["args"] = {"wire_c"};


        ret["io_db"]["outputs"][0]["type"] = "led";
        ret["io_db"]["outputs"][0]["name"] = "led_ab";
        ret["io_db"]["outputs"][0]["args"] = {"output0"};

        ret["io_db"]["outputs"][1]["type"] = "led";
        ret["io_db"]["outputs"][1]["name"] = "led_abc";
        ret["io_db"]["outputs"][1]["args"] = {"output1"};
        std::cout << ret.dump(2);
        return ret;
    }
} // sim