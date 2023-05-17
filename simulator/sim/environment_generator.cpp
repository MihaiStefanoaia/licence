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
        } catch(std::runtime_error& err) {
            std::cout << "Generation failed:\n";
            std::cout << err.what() << "\n";
            std::cout << "loading example...\n";
            return example();
        }
    }

    nlohmann::json environment_generator::example() { //((a&b)&c) - pre-validated
        using namespace nlohmann;
        json ret;

//        ret["has_master_clock"] = false;

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

    nlohmann::json environment_generator::tiny_playground() {
        nlohmann::json ret;

        // wire db:

        ret["wire_db"][0]["name"] = "m_clk";
        ret["wire_db"][0]["level"] = 0;

        ret["wire_db"][1]["name"] = "data_miso_0";
        ret["wire_db"][1]["level"] = 4;
        ret["wire_db"][2]["name"] = "data_miso_1";
        ret["wire_db"][2]["level"] = 4;
        ret["wire_db"][3]["name"] = "data_miso_2";
        ret["wire_db"][3]["level"] = 4;
        ret["wire_db"][4]["name"] = "data_miso_3";
        ret["wire_db"][4]["level"] = 4;

        ret["wire_db"][5]["name"] = "data_mosi_0";
        ret["wire_db"][5]["level"] = 2;
        ret["wire_db"][6]["name"] = "data_mosi_1";
        ret["wire_db"][6]["level"] = 2;
        ret["wire_db"][7]["name"] = "data_mosi_2";
        ret["wire_db"][7]["level"] = 2;
        ret["wire_db"][8]["name"] = "data_mosi_3";
        ret["wire_db"][8]["level"] = 2;

        ret["wire_db"][9]["name"]  = "addr_0";
        ret["wire_db"][9]["level"]  = 2;
        ret["wire_db"][10]["name"] = "addr_1";
        ret["wire_db"][10]["level"] = 2;
        ret["wire_db"][11]["name"] = "addr_2";
        ret["wire_db"][11]["level"] = 2;
        ret["wire_db"][12]["name"] = "addr_3";
        ret["wire_db"][12]["level"] = 2;

        ret["wire_db"][13]["name"] = "port_i_0";
        ret["wire_db"][13]["level"] = 0;
        ret["wire_db"][14]["name"] = "port_i_1";
        ret["wire_db"][14]["level"] = 0;
        ret["wire_db"][15]["name"] = "port_i_2";
        ret["wire_db"][15]["level"] = 0;
        ret["wire_db"][16]["name"] = "port_i_3";
        ret["wire_db"][16]["level"] = 0;

        ret["wire_db"][17]["name"] = "port_o_0";
        ret["wire_db"][17]["level"] = 2;
        ret["wire_db"][18]["name"] = "port_o_1";
        ret["wire_db"][18]["level"] = 2;
        ret["wire_db"][19]["name"] = "port_o_2";
        ret["wire_db"][19]["level"] = 2;
        ret["wire_db"][20]["name"] = "port_o_3";
        ret["wire_db"][20]["level"] = 2;

        ret["wire_db"][21]["name"] = "active";
        ret["wire_db"][21]["level"] = 2;
        ret["wire_db"][22]["name"] = "rw";
        ret["wire_db"][22]["level"] = 2;
        ret["wire_db"][23]["name"] = "ready";
        ret["wire_db"][23]["level"] = 4;

        ret["wire_db"][24]["name"] = "rst";
        ret["wire_db"][24]["level"] = 0;

        ret["wire_db"][25]["name"] = "rst_mem";
        ret["wire_db"][25]["level"] = 2;

        ret["wire_db"][26]["name"] = "interr";
        ret["wire_db"][26]["level"] = 0;

        // array db:

        ret["array_db"][0]["name"] = "data_miso";
        ret["array_db"][0]["size"] = 4;
        ret["array_db"][0]["args"] = {"data_miso_0","data_miso_1","data_miso_2","data_miso_3"};

        ret["array_db"][1]["name"] = "data_mosi";
        ret["array_db"][1]["size"] = 4;
        ret["array_db"][1]["args"] = {"data_mosi_0","data_mosi_1","data_mosi_2","data_mosi_3"};

        ret["array_db"][2]["name"] = "addr";
        ret["array_db"][2]["size"] = 4;
        ret["array_db"][2]["args"] = {"addr_0","addr_1","addr_2","addr_3"};

        ret["array_db"][3]["name"] = "port_i";
        ret["array_db"][3]["size"] = 4;
        ret["array_db"][3]["args"] = {"port_i_0","port_i_1","port_i_2","port_i_3"};

        ret["array_db"][4]["name"] = "port_o";
        ret["array_db"][4]["size"] = 4;
        ret["array_db"][4]["args"] = {"port_o_0","port_o_1","port_o_2","port_o_3"};

        // component_db:
        ret["component_db"][0]["type"] = "tiny_cpu";
        ret["component_db"][0]["name"] = "processor";
        ret["component_db"][0]["level"] = 1;
        ret["component_db"][0]["args"] = {"addr","data_miso","data_mosi","active","rw","ready","port_i","port_o","interr","m_clk","rst"};

        ret["component_db"][1]["type"] = "tiny_mem";
        ret["component_db"][1]["name"] = "memory";
        ret["component_db"][1]["level"] = 3;
        ret["component_db"][1]["args"] = {"addr","data_mosi","data_miso","active","rw","ready","m_clk","rst_mem"};

        ret["component_db"][2]["type"] = "not_module";
        ret["component_db"][2]["name"] = "rst_neg";
        ret["component_db"][2]["level"] = 1;
        ret["component_db"][2]["args"] = {"rst","rst_mem"};

        // inputs:

        ret["io_db"]["inputs"][0]["type"] = "button";
        ret["io_db"]["inputs"][0]["name"] = "b_port_0";
        ret["io_db"]["inputs"][0]["args"] = {"port_i_0"};

        ret["io_db"]["inputs"][1]["type"] = "button";
        ret["io_db"]["inputs"][1]["name"] = "b_port_1";
        ret["io_db"]["inputs"][1]["args"] = {"port_i_1"};

        ret["io_db"]["inputs"][2]["type"] = "button";
        ret["io_db"]["inputs"][2]["name"] = "b_port_2";
        ret["io_db"]["inputs"][2]["args"] = {"port_i_2"};

        ret["io_db"]["inputs"][3]["type"] = "button";
        ret["io_db"]["inputs"][3]["name"] = "b_port_3";
        ret["io_db"]["inputs"][3]["args"] = {"port_i_3"};

        ret["io_db"]["inputs"][4]["type"] = "button";
        ret["io_db"]["inputs"][4]["name"] = "b_interr";
        ret["io_db"]["inputs"][4]["args"] = {"interr"};

        // outputs:

        ret["io_db"]["outputs"][0]["type"] = "led";
        ret["io_db"]["outputs"][0]["name"] = "l_port_0";
        ret["io_db"]["outputs"][0]["args"] = {"port_o_0"};

        ret["io_db"]["outputs"][1]["type"] = "led";
        ret["io_db"]["outputs"][1]["name"] = "l_port_1";
        ret["io_db"]["outputs"][1]["args"] = {"port_o_1"};

        ret["io_db"]["outputs"][2]["type"] = "led";
        ret["io_db"]["outputs"][2]["name"] = "l_port_2";
        ret["io_db"]["outputs"][2]["args"] = {"port_o_2"};

        ret["io_db"]["outputs"][3]["type"] = "led";
        ret["io_db"]["outputs"][3]["name"] = "l_port_3";
        ret["io_db"]["outputs"][3]["args"] = {"port_o_3"};

        // configs (frequency is per frame):

        ret["config_db"]["master_clk"] = "m_clk";
        ret["config_db"]["reactive_only"] = 0;
        ret["config_db"]["sim_frequency_min"] = 2;
        ret["config_db"]["sim_frequency_max"] = 2;
        ret["config_db"]["frame_rate_cap"] = 60;

        return ret;
    }
} // sim