
#include <iostream>
#include "sim/objs/bit.h"
#include "sim/objs/and_module.h"
#include "sim/evaluation_list.h"
#include "word.h"
#include "byte.h"
#include "sim/environment_generator.h"
#include "environment.h"

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    sim::environment env;
    env.start();
    return 0;
}