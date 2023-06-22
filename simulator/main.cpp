
#include <iostream>
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"
#include <thread>
#include "cpu.h"
#include "button.h"
#include "led.h"
#include "sim_monitor.h"
#include "seven_seg.h"
#include "ssd_monitor.h"

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <fstream>
#include <QThread>

///demo sim for ((a&b)&c)

int main(int argc, char** argv){
    if(argc < 2){
        std::cout << "Insufficient arguments. Usage: phiarchsim <path to the file>";
        exit(1);
    }

    QApplication app(argc,argv);

    QThread* thr;
    sim::environment env;
    env.setup(argv[1]);
    auto run = [&env](){
        env.start();
        env.cleanup();
    };

    thr = QThread::create(run);
    thr->start();

    return app.exec();
}