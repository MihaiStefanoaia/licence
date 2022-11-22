//
// Created by Mihai - PC on 11/15/2022.
//

#ifndef SIMULATOR_BIT_ARRAY_H
#define SIMULATOR_BIT_ARRAY_H

#include <cstdint>
#include "bit.h"
#include "../triggering.h"

namespace sim {
    namespace objs {
        class bit_array : public triggering{
        private:
            int hi_index;
            int lo_index;
            uint64_t content;
        public:
            bit_array(int,int,uint64_t);
            bit_array(int hi,int lo) : bit_array(hi,lo,0){};
            uint64_t value_between(int,int) const;
            bool value_at(int) const;
            bit_array& modify_value(int,int,bit_array&,int,int);
            bit_array& modify_value(int,bit&);
            uint64_t get_content() const {return content;};
        };

    } // objs
}

#endif //SIMULATOR_BIT_ARRAY_H
