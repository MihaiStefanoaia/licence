//
// Created by mihai-laptop on 5/23/23.
//

#ifndef SIMULATOR_LEVEL_PROMISE_H
#define SIMULATOR_LEVEL_PROMISE_H

#include <list>
#include <sys/types.h>
#include <set>

namespace sim {

    class level_promise {
    public:
        enum operation : u_int8_t {NONE,BASE,INHERIT,INCREMENT,ADD,MAX,MAX_INC};
        int level = -1;
        bool determined = false;
        operation op;
        std::set<level_promise*> dependencies; // a promise can only have a binary or unary operation
        std::list<level_promise*> downstream_dependencies;
        level_promise(operation op, std::set<level_promise*> dependencies);
        void add_dependency(level_promise* dependency);
        bool determine();
    };

} // sim

#endif //SIMULATOR_LEVEL_PROMISE_H
