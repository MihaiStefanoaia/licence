//
// Created by mihai-pc on 4/1/23.
//

#include <iostream>
#include <cmath>
#include "led.h"

namespace sim {
    namespace objs {
        led::led(std::string s, bit &a) : input(a){
            window = new QWidget();
            window->setWindowTitle(s.c_str());
            grid = new QGridLayout(window);
            light = new QWidget();
            light->setFixedSize(25,25);
            light->setAutoFillBackground(true);
            auto pal = QPalette();
            pal.setColor(QPalette::Window,QColor(0,0,0));
            light->setPalette(pal);
            grid->addWidget(light,0,0,Qt::AlignCenter);

            label = new QLabel();
            label->setText(s.c_str());
            grid->addWidget(label,1,0, Qt::AlignCenter);
            window->setLayout(grid);
        }

        void led::render() {
//            std::cout << "led rendering with " << active_cycles << " of " << all_cycles << " cycles\n";
            float render_val = (float)active_cycles / (float)all_cycles;
            render_val = std::isnormal(render_val) ? render_val : 0;
            int r,g,b;
            r = g = b = (int)(255.0f * gamma_adjust(render_val));
            auto pal = QPalette();
            pal.setColor(QPalette::Window,QColor(r,0,0));
            light->setPalette(pal);
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