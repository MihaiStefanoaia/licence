//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_LED_H
#define SIMULATOR_LED_H

#include "basic_output.h"
#include <QLabel>
#include <QGridLayout>

namespace sim {
    namespace objs {
        class led : public basic_output{
            QWidget* light;
            QGridLayout* grid;
            QLabel* label;
            bit& input;
            int active_cycles;
            int all_cycles;
        public:
            explicit led(std::string s, bit& a);
            void update() override;
            void render() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_LED_H
