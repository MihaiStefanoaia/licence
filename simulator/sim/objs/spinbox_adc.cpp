//
// Created by mihai-pc on 6/25/23.
//

#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include "spinbox_adc.h"

namespace sim {
    namespace objs {
        spinbox_adc::spinbox_adc(const std::string& name, const bit_array &out) : out(out){
            window = new QWidget();

            auto grid = new QGridLayout(window);
            spb = new QSpinBox(window);
            spb->setRange(0,255);
            auto lbl = new QLabel(window);
            grid->addWidget(spb,0,0);
            grid->addWidget(lbl,1,0);
            window->setLayout(grid);
        }

        void spinbox_adc::update() {
            int val = spb->value();
            for(int i = 0; i < 8; i++)
                out[i].set_content(val & (1 << i));
        }

        spinbox_adc *
        spinbox_adc::instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                                 nlohmann::json entry) {
            return new spinbox_adc(std::string(entry["name"]),*array_db[entry["args"][0]]);
        }
    } // sim
} // objs