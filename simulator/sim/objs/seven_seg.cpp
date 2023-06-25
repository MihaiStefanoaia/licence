//
// Created by mihai-pc on 6/16/23.
//

#include <QPainter>
#include <QGraphicsView>
#include "seven_seg.h"

namespace sim {
    namespace objs {
        seven_seg::seven_seg(const bit_array &arr, bit& en) : input(arr), enable(en) {
            monitor = new gui::ssd_monitor();
            window = monitor;
        }

        void seven_seg::update() {
            for(int i = 0; i < 8; i++){
                total[i]++;
                if(enable.get_content() && input[i].get_content())
                    active[i]++;
            }
        }

        void seven_seg::render() {
            for(int i = 0; i < 8; i++){
                float render_val = (float)active[i] / (float)total[i];
                render_val = std::isnormal(render_val) ? render_val : 0;
                int r;
                r = (int)(255.0f * gamma_adjust(render_val));
                monitor->set_segment_color(i, QColor(r, 0, 0));
                active[i] = 0;
                total[i] = 0;
            }
        }
    } // sim
} // objs