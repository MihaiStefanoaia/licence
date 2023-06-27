//
// Created by mihai-laptop on 6/28/23.
//

#ifndef SIMULATOR_DECODER_H
#define SIMULATOR_DECODER_H

#include "evaluable.h"
#include "bit_array.h"

#define NAME_DECODER "decoder"
#define ARGS_SIZES_DECODER {NAME_DECODER,{2,4}}
#define ARGS_DIR_DECODER {NAME_DECODER, {true,false}}

namespace sim {
    namespace objs {

        class decoder : public evaluable {
        bit_array in;
        bit_array out;
        public:
            explicit decoder(const bit_array &in, const bit_array& out);
            void eval() override;
        };

    } // sim
} // objs

#endif //SIMULATOR_DECODER_H
