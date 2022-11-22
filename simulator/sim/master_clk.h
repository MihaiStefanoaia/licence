//
// Created by Mihai - PC on 11/22/2022.
//

#ifndef SIMULATOR_MASTER_CLK_H
#define SIMULATOR_MASTER_CLK_H

#include "triggering.h"
#include "objs/bit.h"

namespace sim {

    class master_clk : public triggering{
    private:
        static master_clk* instance;
        master_clk();
        void eval() override;
    public:
        static master_clk* get_instance();
        static void step();
    };
    master_clk* master_clk::instance = nullptr;

} // sim

#endif //SIMULATOR_MASTER_CLK_H
