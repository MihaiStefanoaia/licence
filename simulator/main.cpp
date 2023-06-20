
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
    QApplication app(argc,argv);

    QWidget mainWidget;
    QGridLayout layout(&mainWidget);

    sim::gui::ssd_monitor monitor_0;
    monitor_0.set_segment_color(0, Qt::red);
    monitor_0.set_segment_color(1, Qt::black);
    monitor_0.set_segment_color(2, Qt::blue);
    monitor_0.set_segment_color(3, Qt::green);
    monitor_0.set_segment_color(4, Qt::gray);
    monitor_0.set_segment_color(5, Qt::magenta);
    monitor_0.set_segment_color(6, Qt::cyan);
    monitor_0.set_segment_color(7, Qt::darkGreen);

    sim::gui::ssd_monitor monitor_1;
    monitor_1.set_segment_color(0, Qt::red);
    monitor_1.set_segment_color(1, Qt::black);
    monitor_1.set_segment_color(2, Qt::blue);
    monitor_1.set_segment_color(3, Qt::green);
    monitor_1.set_segment_color(4, Qt::gray);
    monitor_1.set_segment_color(5, Qt::magenta);
    monitor_1.set_segment_color(6, Qt::cyan);
    monitor_1.set_segment_color(7, Qt::darkGreen);

    layout.addWidget(&monitor_0, 0, 0);
    layout.addWidget(&monitor_1, 0, 1);
    mainWidget.setLayout(&layout);
    mainWidget.show();

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

//    sim::objs::bit en;
//    sim::objs::bit_array con(8);
//    for(int i = 0; i < 8; i++)
//        con.connect(en,i);
//
//    sim::objs::seven_seg ssd(con,en);
//
//    ssd.get_window()->show();
//
//    ssd.render();

    return app.exec();
}