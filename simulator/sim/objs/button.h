//
// Created by mihai-laptop on 3/22/23.
//

#ifndef SIMULATOR_BUTTON_H
#define SIMULATOR_BUTTON_H

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include "bit.h"
#include "basic_input.h"

namespace sim {
    namespace objs {

        class button : public basic_io {
            QPushButton* btn;
            QCheckBox* hold;
            QLabel* hold_lbl;
            QGridLayout* hold_grid;
            QGridLayout* main_grid;
            bit &output;
            bool buffer = false;
        public:
            explicit button(const std::string& s, bit &a);
            ~button() override;
            void press();
            void release();
            void update() override;
        };
    } // sim
} // objs

#endif //SIMULATOR_BUTTON_H
