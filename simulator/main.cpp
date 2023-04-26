
#include <iostream>
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    std::cout << sim::environment_generator::generate("example.ndl").dump(2) << '\n';
    sim::environment env;
    env.start();
    return 0;
}