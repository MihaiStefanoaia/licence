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
        if((*lst)[level] == nullptr)
            (*lst)[level] = new std::list<evaluable*>();
        (*lst)[level]->push_back(e);
    }

    void evaluation_list::eval() {
        for(auto kv : *lst){
//            std::cout << kv.first << std::endl;
            for(auto el : *(kv.second)) {
                if (el->is_flagged()) {
                    std::cout<<"evaluating at level " << kv.first <<"...\n";
                    el->eval();
                }
            }
        }
    }

    evaluation_list::evaluation_list() : evaluable() {
        lst = new std::map<int,std::list<evaluable*>*>;
    }

    evaluation_list::~evaluation_list() {
        for (auto kv : *lst){
            delete kv.second;
        }
        delete lst;
    }

    void evaluation_list::add_on_expected_level(evaluable* e) {
        this->add_on_level(e->get_expected_level(),e);
    }
} // sim