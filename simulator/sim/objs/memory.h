//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "bit.h"
#include "byte.h"
#include "word.h"
#include "json.hpp"
#include <sys/types.h>
#include <map>

#define NAME_MEMORY "memory"
#define ARGS_SIZES_MEMORY {NAME_MEMORY,{16  ,8   ,1   ,1   ,1   ,8    ,1}}
#define ARGS_DIR_MEMORY   {NAME_MEMORY,{true,true,true,true,true,false,false}}


namespace sim {
    namespace objs {

        class memory : public evaluable{
        private:
            //inputs
            bit_array addr_i;
            bit_array data_i;
            bit& rw;
            bit& req_enable;
            bit& CLK;

            //outputs
            bit_array data_o;
            bit& ready;

            //internal variables
            enum st{LISTENING, DONE};
            st state = LISTENING;
            u_int8_t mem[1<<16] = {0};
        public:
            void eval() override;
            void flag_for_eval(triggering*) override;
            memory(const bit_array &addr_i, const bit_array &data_i, bit &rw, bit &req_enable, bit &clk, const bit_array &data_o,bit &ready);
            static memory* instantiate(std::map<std::string, bit *> &wire_db, std::map<std::string, bit_array *> &array_db,
                                       nlohmann::json entry);
            void bootload(const std::string& path);
        };

    } // sim
} // objs

#endif //SIMULATOR_MEMORY_H
