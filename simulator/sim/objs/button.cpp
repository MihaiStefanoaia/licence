//
// Created by mihai-laptop on 3/22/23.
//

#include <iostream>
#include "button.h"

namespace sim {
    namespace objs {

        button::button(const std::string& s, bit &a) : output(a){
            window = new QWidget();
            main_grid = new QGridLayout();
            btn = new QPushButton(s.c_str());
            hold_grid = new QGridLayout();
            hold = new QCheckBox();
            hold_lbl = new QLabel("hold");

            hold_grid->addWidget(hold,0,0,Qt::AlignCenter);
            hold_grid->addWidget(hold_lbl,0,1,Qt::AlignLeft);

            main_grid->addWidget(btn,0,0,Qt::AlignCenter);
            main_grid->addLayout(hold_grid,1,0,Qt::AlignCenter);

            window->setLayout(main_grid);
            btn->setFixedSize(100,100);
            window->setWindowTitle(s.c_str());
            auto toggle_func = [this](){
                if(this->btn->isChecked()) {
                    this->press();
                }else {
                    this->release();
                }
            };

            auto press_func = [this](){
                if(!this->btn->isCheckable())
                    this->press();
            };
            auto release_func = [this](){
                if(!this->btn->isCheckable())
                    this->release();
            };

            auto hold_func = [this](){
                if(this->hold->isChecked()){
                    this->btn->setCheckable(true);
                } else {
                    this->btn->setCheckable(false);
                }
            };

            QPushButton::connect(btn, &QPushButton::pressed, window, press_func);
            QPushButton::connect(btn, &QPushButton::released, window, release_func);
            QPushButton::connect(btn, &QPushButton::toggled, window, toggle_func);
            QCheckBox::connect(hold, &QCheckBox::stateChanged, window, hold_func);
        }

        void button::press() {
            auto pal = QPalette();
            btn->setAutoFillBackground(true);
            pal.setColor(QPalette::Button,Qt::green);
            btn->setPalette(pal);
            std::cout << "NOW PRESSED\n";
            buffer = true;
        }

        void button::release() {
            auto pal = QPalette();
            btn->setAutoFillBackground(true);
            pal.setColor(QPalette::Button,Qt::white);
            btn->setPalette(pal);
            std::cout << "NOW RELEASED\n";
            buffer = false;
        }

        void button::update() {
            output.set_content(buffer);
        }

        button::~button() {
            delete btn;
            delete hold;
            delete hold_grid;
            delete main_grid;
            delete window;
        }

    } // sim
} // objs