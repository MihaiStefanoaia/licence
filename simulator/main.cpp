
#include <iostream>
#include "sim/objs/bit.h"
#include "sim/objs/and_module.h"
#include "sim/evaluation_list.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    sim::evaluation_list ev_list;
    std::string input;
    auto* a_bit = new sim::objs::bit(false);
    auto* b_bit = new sim::objs::bit(false);
    auto* c_bit = new sim::objs::bit(false);
    auto* a = new sim::objs::wire(a_bit);
    auto* b = new sim::objs::wire(b_bit);
    auto* c = new sim::objs::wire(c_bit);
    sim::objs::wire* out_1;
    sim::objs::wire* out;
    sim::objs::and_module and_mod(a, b, out_1);
    sim::objs::and_module and_out(c, out_1, out);
    std::cout << a_bit->get_expected_level() << '\n'
              << b_bit->get_expected_level() << '\n'
              << a->get_expected_level() << '\n'
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
        std::cout<<">>>";
        char buf[100];
        std::cin.getline(buf,100);
        input = buf;
        if(input == std::string ("set a")){
            a_bit->set_content(true);
        } else if(input == std::string ("set b")){
            b_bit->set_content(true);
        } else if(input == std::string ("set c")){
            c_bit->set_content(true);
        } else if(input == std::string ("unset a")){
            a_bit->set_content(false);
        } else if(input == std::string ("unset b")){
            b_bit->set_content(false);
        } else if(input == std::string ("unset c")){
            c_bit->set_content(false);
        } else if(input == std::string("exit")){
            std::cout<<"exiting\n";
            break;
        } else {
            std::cout<<"invalid command (set or unset only)\n";
            continue;
        }

        ev_list.eval();

        std::cout<<"a : "<< ((a_bit->get_content()) ? "true" : "false") << '\n';
        std::cout<<"b : "<< ((b_bit->get_content()) ? "true" : "false") << '\n';
        std::cout<<"c : "<< ((c_bit->get_content()) ? "true" : "false") << '\n';
        std::cout << "o1: " << ((out_1->get_content()) ? "true" : "false") << '\n';
        std::cout << "o2: " << ((out->get_content()) ? "true" : "false") << '\n';
    }

    return 0;
}