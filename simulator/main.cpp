
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
#include <QThread>

///demo sim for ((a&b)&c)

int main(int argc, char** argv){
    QApplication app(argc,argv);

    QThread* thr;
    sim::environment env;
    env.setup("tiny_playground.ndl");
    auto run = [&env](){
        env.start();
    };

    thr = QThread::create(run);
    thr->start();
//    auto b = new sim::objs::bit();
//    auto a = new sim::objs::button("bruh",*b);
//    a->get_window()->show();
//    auto s = new sim::objs::button("come on",*b);
//    s->get_window()->show();
    return app.exec();
}