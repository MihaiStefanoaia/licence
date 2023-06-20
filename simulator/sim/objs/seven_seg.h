//
// Created by mihai-pc on 6/16/23.
//

#ifndef SIMULATOR_SEVEN_SEG_H
#define SIMULATOR_SEVEN_SEG_H

#include <QGraphicsScene>
#include "evaluable.h"
#include "bit_array.h"
#include "basic_output.h"

#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_H 7


namespace sim {
    namespace objs {

        class seven_seg : public basic_output{
            bit_array input;
            bit& enable;
            int active[8];
            int total[8];
            QPolygon segments[8];

            QGraphicsScene* graphics_scene;
            QGraphicsView* view;
        public:
            explicit seven_seg(const bit_array&, bit&);
            void update() override;
            void render() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_SEVEN_SEG_H
