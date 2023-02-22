
#include <iostream>
#include "sim/objs/bit.h"
#include "sim/objs/and_module.h"
#include "sim/evaluation_list.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    sim::evaluation_list ev_list;
    std::string input;
    auto* a = new sim::objs::bit(false);
    auto* b = new sim::objs::bit(false);
    auto* c = new sim::objs::bit(false);
    sim::objs::bit* out_1;
    sim::objs::bit* out;
    sim::objs::and_module and_mod(a, b, out_1);
    sim::objs::and_module and_out(c, out_1, out);
    std::cout << a->get_expected_level() << '\n'
              << b->get_expected_level() << '\n'
              << and_mod.get_expected_level() << '\n'
              << out_1->get_expected_level() << '\n';

    ev_list.add_on_expected_level(a);
    ev_list.add_on_expected_level(b);
    ev_list.add_on_expected_level(c);
    ev_list.add_on_expected_level(&and_mod);
    ev_list.add_on_expected_level(&and_out);
    ev_list.add_on_expected_level(out_1);
    ev_list.add_on_expected_level(out);

    while(input != "exit"){
        system("clear");
        std::cout << "a      : "<< ((a->get_content()) ? "true" : "false") << '\n';
        std::cout << "b      : "<< ((b->get_content()) ? "true" : "false") << '\n';
        std::cout << "c      : "<< ((c->get_content()) ? "true" : "false") << '\n';
        std::cout << "a&b    : " << ((out_1->get_content()) ? "true" : "false") << '\n';
        std::cout << "(a&b)&c: " << ((out->get_content()) ? "true" : "false") << '\n';
        std::cout<<">>>";
        char buf[100];
        std::cin.getline(buf,100);
        input = buf;
        if(input == std::string ("set a")){
            a->set_content(true);
        } else if(input == std::string ("set b")){
            b->set_content(true);
        } else if(input == std::string ("set c")){
            c->set_content(true);
        } else if(input == std::string ("unset a")){
            a->set_content(false);
        } else if(input == std::string ("unset b")){
            b->set_content(false);
        } else if(input == std::string ("unset c")){
            c->set_content(false);
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