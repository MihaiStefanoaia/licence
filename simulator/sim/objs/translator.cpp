//
// Created by mihai-laptop on 6/28/23.
//

#include "translator.h"

namespace sim {
    namespace objs {
        void translator::eval() {
            flag = nullptr;
            int index = 0;
            for(int i = 0; i < 4; i++)
                index = (index << 1) | (in[3 - i].get_content() ? 1 : 0);
            auto output = tr[index];
            for(int i = 0; i < 8; i++)
                out[i].set_content(output & (1<<i));
        }

        translator::translator(const bit_array &in, const bit_array& out) : in(in), out(out) {
            for(int i = 0; i < 4; i++)
                in[i].add_trigger(this,triggering::BOTH);
        }
    } // sim
} // objs