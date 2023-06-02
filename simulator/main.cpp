
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

    auto bit = new sim::objs::bit();
    auto btn = new sim::objs::button("the button",*bit);
    auto led = new sim::objs::led("the_led",*bit);
    btn->get_window()->show();
    led->get_window()->show();
    return app.exec();
}