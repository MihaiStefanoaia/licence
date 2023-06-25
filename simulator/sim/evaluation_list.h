//
// Created by Mihai - PC on 11/15/2022.
//

#ifndef SIMULATOR_EVALUATION_LIST_H
#define SIMULATOR_EVALUATION_LIST_H

#include <map>
#include <list>
#include "evaluable.h"

namespace sim {

    class evaluation_list : public evaluable{
    private:
        std::map<int,std::list<evaluable*>> lst;
        evaluable** cmt;
        int size;
        bool committed = false;
    public:
        evaluation_list();
        ~evaluation_list() override;
        void commit();
        void add_on_level(int, evaluable*);
        void add_on_expected_level(evaluable*);
        void eval() override;
        void full_eval();
    };

} // sim

#endif //SIMULATOR_EVALUATION_LIST_H
