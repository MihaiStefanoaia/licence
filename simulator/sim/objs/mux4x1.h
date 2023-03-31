//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MUX4X1_H
#define SIMULATOR_MUX4X1_H

#include "bit.h"

namespace sim {
    namespace objs {

        class mux4x1 : protected evaluable{
        private:
            bit& a;
            bit& b;
            bit& c;
            bit& d;
            bit& sel0;
            bit& sel1;
            bit& out;
        public:
            mux4x1(bit&,bit&,bit&,bit&,bit&,bit&,bit&);
            ~mux4x1();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_MUX4X1_H
