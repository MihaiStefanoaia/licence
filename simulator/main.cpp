
#include <iostream>
#include "sim/objs/bit.h"
#include "sim/objs/and_module.h"
#include "sim/evaluation_list.h"
#include "word.h"
#include "byte.h"
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

///demo sim for ((a&b)&c)
int main(int argc, char** argv){
    sim::transpiler::transpile("example.ndl");
    QApplication app(argc,argv);

    auto *window = new QWidget();
    auto *grid = new QGridLayout();
    auto *label = new QLabel("Hello world");
    label->setMinimumSize(200,50);
    label->setAlignment(Qt::AlignCenter);
    label->resize(250,100);


    auto on_click = [](){
        std::cout << "clicked\n";
    };
    auto *button = new QPushButton("click me");
    button->setMinimumSize(100,50);
    QPushButton::connect(button,&QPushButton::clicked, window ,on_click);

    grid->addWidget(label);
    grid->addWidget(button);
    window->setLayout(grid);
    window->show();
    //    sim::environment env;
//    env.start();
    return app.exec();
}