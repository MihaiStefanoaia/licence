//
// Created by mihai-laptop on 6/26/23.
//

#include "pwm_generator.h"

namespace sim {
    namespace objs {
        pwm_generator::pwm_generator(const bit_array &in, bit &out, bit &CLK) :
        in(in), out(out), CLK(CLK){
            CLK.add_trigger(this,triggering::POSITIVE);
        }

        void pwm_generator::eval() {
            if(counter == 0){
                counter = 255;
                for(int i = 7; i >= 0; i--){
                    duty <<= 1;
                    duty |= in[i].get_content();
                }
            }

            out.set_content(duty > 0);
            counter--;
            if(duty > 0)
                duty--;
        }

        pwm_generator *pwm_generator::instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                                   nlohmann::json entry) {
            auto args = entry["args"];
            return new pwm_generator(*array_db[args[0]],*wire_db[args[1]],*wire_db[args[2]]);
        }
    } // sim
} // objs