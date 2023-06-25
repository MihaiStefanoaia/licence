//
// Created by Mihai - PC on 11/15/2022.
//

#include <iostream>
#include "evaluation_list.h"

namespace sim {
    void evaluation_list::add_on_level(int level, evaluable* e) {
        if (e == nullptr) {
            throw std::invalid_argument("Cannot add nullptr to evaluation list");
        }
        if(lst.count(level) == 0)
            lst[level] = {};
        lst[level].push_back(e);
    }

    void evaluation_list::eval() {
        if(!committed){
            for(const auto& kv : lst){
//                std::cout << kv.first << std::endl;
                for(auto el : kv.second) {
                    if (el->is_flagged()) {
                        el->eval();
                    }
                }
            }
        } else {
            for(int i = 0; i < size; i++){
                if(cmt[i]->is_flagged())
                    cmt[i]->eval();
            }
        }
    }

    evaluation_list::evaluation_list() : evaluable() {
    }

    evaluation_list::~evaluation_list(){
        if(committed)
            delete[] cmt;
    }

    void evaluation_list::add_on_expected_level(evaluable* e) {
        this->add_on_level(e->get_expected_level(),e);
    }

    void evaluation_list::commit() {
        if(committed)
            return;
        committed = true;
        size = 0;
        for(const auto& kvp : lst){
            size += kvp.second.size();
        }
        cmt = new sim::evaluable * [size];
        int i = 0;
        for(const auto& kvp : lst){
            for(const auto& ev : kvp.second){
                cmt[i] = ev;
                i++;
            }
        }
    }

    void evaluation_list::full_eval() {
        if(!committed){
            for(const auto& kv : lst){
//                std::cout << kv.first << std::endl;
                for(auto el : kv.second) {
                    if (el->is_flagged()) {
                        el->eval();
                    }
                }
            }
        } else {
            for(int i = 0; i < size; i++){
                if(cmt[i]->is_flagged())
                    cmt[i]->eval();
            }
        }
    }
} // sim