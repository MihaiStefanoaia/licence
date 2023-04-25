
#include <iostream>
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    nlohmann::json test;
    test = "hello";
    std::cout << test.dump();
    sim::transpiler trs;
    trs.transpile("example.ndl");
    std::cout << trs.ret.dump(2) << '\n';
    sim::environment env;
    env.start();
    return 0;
}