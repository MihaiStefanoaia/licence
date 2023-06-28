//
// Created by mihai-laptop on 6/28/23.
//

#ifndef SIMULATOR_TRANSLATOR_H
#define SIMULATOR_TRANSLATOR_H

#include "evaluable.h"
#include "bit_array.h"

#define NAME_TRANSLATOR "translator_ssd"
#define ARGS_SIZES_TRANSLATOR {NAME_TRANSLATOR, {4,8}}
#define ARGS_DIR_TRANSLATOR {NAME_TRANSLATOR, {true,false}}

namespace sim {
    namespace objs {

        class translator : public evaluable {
            bit_array in;
            bit_array out;
            const u_int16_t tr[16] = {0x3F, 0x06, 0x5B, 0x4F,
                                      0x66, 0x6D, 0x7D, 0x07,
                                      0x7F, 0x7D, 0x77, 0x7C,
                                      0x58, 0x5E, 0x79, 0x71};
        public:
            explicit translator(const bit_array &in, const bit_array& out);

            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_TRANSLATOR_H
