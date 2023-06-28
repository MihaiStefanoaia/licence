//
// Created by Mihai - PC on 11/24/2022.
//

#include "bootloader.h"
#include <QFileDialog>

namespace sim {
    namespace gui {
        bootloader::bootloader(std::string name, objs::memory &mem) : mem(mem){
            lbl = new QLabel(name.c_str());
            load = new QPushButton("Load ROM");
            layout = new QGridLayout();
            layout->addWidget(lbl,0,0);
            layout->addWidget(load,1,0);
            window = new QWidget();
            window->setLayout(layout);

            auto click_fun = [this](){
                auto str = QFileDialog::getOpenFileName(window,"Open ROM");
                this->mem.bootload(str.toStdString());
            };

            QPushButton::connect(load,&QPushButton::clicked,click_fun);
        }

        void bootloader::update() {

        }
    } // sim
} // objs