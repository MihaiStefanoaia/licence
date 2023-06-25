//
// Created by mihai-pc on 6/25/23.
//

#ifndef SIMULATOR_SPINBOX_ADC_H
#define SIMULATOR_SPINBOX_ADC_H

#include "basic_input.h"
#include "bit_array.h"
#include "json.hpp"

#define NAME_SPINBOX_ADC "spinbox_adc"
#define ARGS_SIZES_SPINBOX_ADC {NAME_SPINBOX_ADC, {8}}
#define ARGS_DIR_SPINBOX_ADC   {NAME_SPINBOX_ADC, {false}}


namespace sim {
    namespace objs {

        class spinbox_adc : public basic_input{
            bit_array out;
            QSpinBox* spb;
        public:
            spinbox_adc(const std::string& name, const bit_array& out);
            void update() override;
            static spinbox_adc *instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                                    nlohmann::json entry);
        };

    } // sim
} // objs

#endif //SIMULATOR_SPINBOX_ADC_H
