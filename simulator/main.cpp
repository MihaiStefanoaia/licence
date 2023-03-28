
#include <iostream>
#include "sim/objs/bit.h"
#include "sim/objs/and_module.h"
#include "sim/evaluation_list.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    using namespace sim::objs;
    sim::evaluation_list ev_list;
    std::string input;
    std::map<std::string,bit*> wire_db;
    wire_db["a"] = new bit();
    wire_db["b"] = new bit();
    wire_db["c"] = new bit();
    wire_db["ot"] = new bit();
    wire_db["o"] = new bit();

    bit& a = *wire_db["a"];
    bit& b = *wire_db["b"];
    bit& c = *wire_db["c"];
    bit& ot = *wire_db["ot"];
    bit& o = *wire_db["o"];
    sim::objs::and_module and_mod(a, b, ot);
    sim::objs::and_module and_out(c, ot, o);
    std::cout << a.get_expected_level() << '\n'
              << b.get_expected_level() << '\n'
              << and_mod.get_expected_level() << '\n'
              << ot.get_expected_level() << '\n';

    ev_list.add_on_expected_level(&and_mod);
    ev_list.add_on_expected_level(&and_out);

    for(auto const& kvp : wire_db){
        ev_list.add_on_expected_level(kvp.second);
    }

    while(input != "exit"){
        system("clear");
        std::cout << "a      : "<< ((a.get_content()) ? "true" : "false") << '\n';
        std::cout << "b      : "<< ((b.get_content()) ? "true" : "false") << '\n';
        std::cout << "c      : "<< ((c.get_content()) ? "true" : "false") << '\n';
        std::cout << "a&b    : " << ((ot.get_content()) ? "true" : "false") << '\n';
        std::cout << "(a&b)&c: " << ((o.get_content()) ? "true" : "false") << '\n';
        std::cout<<">>>";
        char buf[100];
        std::cin.getline(buf,100);
        input = buf;
        if(input == std::string ("set a")){
            a.set_content(true);
        } else if(input == std::string ("set b")){
            b.set_content(true);
        } else if(input == std::string ("set c")){
            c.set_content(true);
        } else if(input == std::string ("unset a")){
            a.set_content(false);
        } else if(input == std::string ("unset b")){
            b.set_content(false);
        } else if(input == std::string ("unset c")){
            c.set_content(false);
        } else if(input == std::string("exit")){
            std::cout<<"exiting\n";
            break;
        } else {
            std::cout<<"invalid command (set or unset only)\n";
            continue;
        }

        ev_list.eval();
    }

    return 0;
}