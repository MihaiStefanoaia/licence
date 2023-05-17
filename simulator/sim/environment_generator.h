//
// Created by mihai-pc on 3/31/23.
//

#ifndef SIMULATOR_ENVIRONMENT_GENERATOR_H
#define SIMULATOR_ENVIRONMENT_GENERATOR_H

#include "json.hpp"

namespace sim {

    class environment_generator {
    public:
        static nlohmann::json generate(const std::string &path);
        static nlohmann::json example();
        static nlohmann::json tiny_playground();
    };

} // sim

#endif //SIMULATOR_ENVIRONMENT_GENERATOR_H
