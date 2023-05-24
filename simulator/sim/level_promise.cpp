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
        for(auto dependency : dependencies) {
        }
        switch (op) {
            case BASE:
                level = 0;
                determined = true;
                break;
            case INHERIT:
                if((*dependencies.begin())->determined) {
                    level = (*dependencies.begin())->level;
                    determined = true;
                }
                break;
            case INC:
                if((*dependencies.begin())->determined) {
                    level = (*dependencies.begin())->level + 1;
                    determined = true;
                }
                break;
            case ADD:
                level = 0;
                for(auto dependency : dependencies) {
                    if(dependency->determined)
                        level += dependency->level;
                    else {
                        success = false;
                        level = -1;
                        break;
                    }
                }
                determined = success;
                break;
            case MAX:
                level = 0;
                for(auto dependency : dependencies) {
                    if(dependency->determined) {
                        level = std::max(level, dependency->level);
                    } else {
                        success = false;
                        level = -1;
                        break;
                    }
                }
                determined = success;
                break;
            case MAX_INC:
                level = 0;
                for(auto dependency : dependencies) {
                    if(dependency->determined)
                        level = std::max(level, dependency->level);
                    else {
                        success = false;
                        level = -2;
                        break;
                    }
                }
                level++;
                determined = success;
                break;
            case NONE:
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