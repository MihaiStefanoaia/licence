//
// Created by mihai-laptop on 6/28/23.
//

#include "decoder.h"

namespace sim {
    namespace objs {
        decoder::decoder(const bit_array &in, const bit_array& out) : in(in), out(out) {
            in[0].add_trigger(this,triggering::BOTH);
            in[1].add_trigger(this,triggering::BOTH);
        }

        void decoder::eval() {
            out[0].set_content(!in[0].get_content() && !in[1].get_content());
            out[1].set_content( in[0].get_content() && !in[1].get_content());
            out[2].set_content(!in[0].get_content() &&  in[1].get_content());
            out[3].set_content( in[0].get_content() &&  in[1].get_content());
        }
    } // sim
} // objs