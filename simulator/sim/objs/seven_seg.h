//
// Created by mihai-pc on 6/16/23.
//

#ifndef SIMULATOR_SEVEN_SEG_H
#define SIMULATOR_SEVEN_SEG_H

#include <QGraphicsScene>
#include "evaluable.h"
#include "bit_array.h"
#include "basic_output.h"
#include "ssd_monitor.h"

#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_H 7

#define NAME_SSD "ssd"
#define ARGS_SIZES_SSD {NAME_SSD, {8   ,1}}
#define ARGS_DIR_SSD   {NAME_SSD, {true,true}}


namespace sim {
    namespace objs {

        class seven_seg : public basic_output{
            bit_array input;
            bit& enable;
            int active[8] = {0};
            int total[8] = {0};
            gui::ssd_monitor* monitor;
        public:
            explicit seven_seg(const bit_array&, bit&);
            void update() override;
            void render() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_SEVEN_SEG_H
