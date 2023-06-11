
#include <iostream>
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"
#include <thread>
#include "cpu.h"
#include "button.h"
#include "led.h"

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <fstream>

///demo sim for ((a&b)&c)

int main(int argc, char** argv){
    QApplication app(argc,argv);

//    sim::transpiler::transpile("tiny_playground.ndl");
    auto run = [](){
        sim::environment env;
        env.start("tiny_playground.ndl");
    };
    std::thread th(run);
    th.detach();
    return app.exec();
}