//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_MUX2X1_H
#define SIMULATOR_MUX2X1_H

#include "bit.h"

#define NAME_MUX_2x1 "mux_2x1"
#define ARGS_SIZES_MUX_2x1 {NAME_MUX_2x1, {1   ,1   ,1   ,1}}
#define ARGS_DIR_MUX_2x1   {NAME_MUX_2x1, {true,true,true,false}}


namespace sim {
    namespace objs {

        class mux2x1 : public evaluable{
        private:
            bit& a;
            bit& b;
            bit& sel;
            bit& out;
        public:
            mux2x1(bit&,bit&,bit&,bit&);
            ~mux2x1();
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_MUX2X1_H
