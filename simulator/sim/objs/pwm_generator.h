//
// Created by mihai-laptop on 6/26/23.
//

#ifndef SIMULATOR_PWM_GENERATOR_H
#define SIMULATOR_PWM_GENERATOR_H

#include <map>
#include "evaluable.h"
#include "bit_array.h"
#include "json.hpp"

#define NAME_PWM "pwm_generator"
#define ARGS_SIZES_PWM {NAME_PWM, {8   ,1    ,1}}
#define ARGS_DIR_PWM   {NAME_PWM, {true,false,true}}

namespace sim {
    namespace objs {

        class pwm_generator : public evaluable {
            bit_array in;
            bit& out;
            bit& CLK;
            int duty = 0;
            int counter = 0;
        public:
            pwm_generator(const bit_array& in, bit& out, bit& CLK);
            void eval() override;
            static pwm_generator *instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                                            nlohmann::json entry);
        };

    } // sim
} // objs

#endif //SIMULATOR_PWM_GENERATOR_H
