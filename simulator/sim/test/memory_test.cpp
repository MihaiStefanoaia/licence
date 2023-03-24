//
// Created by mihai-laptop on 3/13/23.
//

#include "memory_test.h"

namespace sim {
    namespace test {
        void memory_test::write(u_int16_t addr, u_int8_t val) {

        }

        u_int8_t memory_test::read(u_int16_t addr) {
            CE->set_content(true);
            rw->set_content(false);
            for(int i = 0; i < 16; i++){
                addr_i[i]->set_content((addr >> i) & 1);
            }
            step();
            step();
            u_int8_t ret = 0;
            for(int i = 0; i < 8; i++){
                ret *= 2;
                ret &= val_o[i]->get_content();
            }
            return ret;
        }

        void memory_test::reset() {
            RST->set_content(true);
            evl.eval();
            RST->set_content(false);
            evl.eval();
        }

        memory_test::memory_test() {
            CLK = new bit(true);
            RST = new bit();
            CE = new bit();
            rw = new bit();
            for(auto& a : addr_i){
                a = new bit();
            }
            for(auto& a : val_i){
                a = new bit();
            }
            mem = new memory<16>(addr_i,val_i,rw,CE,CLK,RST,val_o,ready);
            evl.add_on_expected_level(CLK);
            evl.add_on_expected_level(RST);
            evl.add_on_expected_level(CE);
            evl.add_on_expected_level(rw);
            for(auto& a : addr_i){
                evl.add_on_expected_level(a);
            }
            for(auto& a : val_i){
                evl.add_on_expected_level(a);
            }
            evl.add_on_expected_level(mem);

        }

        memory_test::~memory_test() {
            delete mem;
            delete CLK;
            delete RST;
            delete CE;
            delete rw;
            for(auto& a : addr_i){
                delete a;
            }
            for(auto& a : val_i){
                delete a;
            }
        }

        void memory_test::step() {
            CLK->set_content(!CLK->get_content());
            if(!CLK->get_content())
                evl.eval();
        }
    } // sim
} // test