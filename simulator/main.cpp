
#include <iostream>
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"
#include <thread>
#include "cpu.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    std::cout << sim::environment_generator::generate("example.ndl").dump(2) << '\n';
    sim::environment env;
    auto server_core = [](sim::environment* env){
        env->start();
    };
    std::thread server(server_core, &env);
    server.join();

    std::cout << sizeof(sim::objs::cpu) << " " << alignof(sim::objs::cpu);
    return 0;
}