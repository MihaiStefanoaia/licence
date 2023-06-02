//
// Created by mihai-laptop on 5/23/23.
//

#include "level_promise.h"

#include <utility>
#include <stdexcept>

namespace sim {
    level_promise::level_promise(level_promise::operation op, std::set<level_promise*> dependencies = {}) {
        if(dependencies.empty() && !(op == BASE || op == NONE))
            throw std::runtime_error("Having no upstream dependencies is only allowed for 'BASE' dependencies (inputs)");
        this->dependencies = std::move(dependencies);
        this->op = op;
    }

    bool level_promise::determine() {
        if(determined)
            return true;
        bool success = true;
        switch (op) {
            case BASE:
                value = 0;
                determined = true;
                break;
            case INHERIT:
                if((*dependencies.begin())->determined) {
                    value = (*dependencies.begin())->value;
                    determined = true;
                }
                break;
            case INCREMENT:
                if((*dependencies.begin())->determined) {
                    value = (*dependencies.begin())->value + 1;
                    determined = true;
                }
                break;
            case ADD:
                value = 0;
                for(auto dependency : dependencies) {
                    if(dependency->determined)
                        value += dependency->value;
                    else {
                        success = false;
                        value = -1;
                        break;
                    }
                }
                determined = success;
                break;
            case MAX:
                value = 0;
                for(auto dependency : dependencies) {
                    if(dependency->determined) {
                        value = std::max(value, dependency->value);
                    } else {
                        success = false;
                        value = -1;
                        break;
                    }
                }
                determined = success;
                break;
            case MAX_INC:
                value = 0;
                for(auto dependency : dependencies) {
                    if(dependency->determined)
                        value = std::max(value, dependency->value);
                    else {
                        success = false;
                        value = -2;
                        break;
                    }
                }
                value++;
                determined = success;
                break;
            case NONE:
                determined = success;
                break;
            default:
                break;
        }
        return determined;
    }

    void level_promise::add_dependency(level_promise *dependency) {
        if(this == dependency || dependencies.count(dependency))
            throw std::runtime_error("A promise cannot depend on itself");
        dependencies.insert(dependency);
    }
} // sim