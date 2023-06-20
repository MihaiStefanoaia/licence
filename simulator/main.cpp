
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
    QApplication app(argc,argv);

//    QThread* thr;
//    sim::environment env;
//    env.setup("tiny_playground.ndl");
//    auto run = [&env](){
//        env.start();
//        env.cleanup();
//    };
//
//    thr = QThread::create(run);
//    thr->start();

    sim::objs::bit en;
    sim::objs::bit_array con(8);
    for(int i = 0; i < 8; i++)
        con.connect(en,i);

    sim::objs::seven_seg ssd(con,en);

    ssd.get_window()->show();

    ssd.render();

    return app.exec();
}