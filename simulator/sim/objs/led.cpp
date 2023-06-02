//
// Created by mihai-pc on 4/1/23.
//

#include "led.h"

namespace sim {
    namespace objs {
        led::led(std::string s, bit &a) : input(a){
            window = new QWidget();
            window->setFixedSize(100,100);
            window->setAutoFillBackground(true);
            auto pal = QPalette();
            pal.setColor(QPalette::Window,QColor(0,0,0));
            window->setPalette(pal);
            window->setWindowTitle(s.c_str());
        }

        void led::render() {
            float render_val = (float)active_cycles / (float)all_cycles;
            int r,g,b;
            r = g = b = 255 * render_val;
            auto pal = QPalette();
            pal.setColor(QPalette::Window,QColor(r,g,b));
            all_cycles = 0;
            active_cycles = 0;
        }

        void led::update() {
            all_cycles++;
            if(input.get_content())
                active_cycles++;
        }
    } // sim
} // objs