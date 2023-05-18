
#include <iostream>
#include "sim/environment_generator.h"
#include "environment.h"
#include "transpiler.h"
#include <thread>
#include "cpu.h"

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>

///demo sim for ((a&b)&c)

int main(int argc, char** argv){
    sim::transpiler::transpile("tiny_playground.ndl");
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
    auto* other_window = new QWidget();

    auto* other_button = new QPushButton("it works hopefully");
    other_button->setMinimumSize(200,50);


    auto pressed_func = [&other_button](){
        if(!other_button->isCheckable()) {
            std::cout << "PRESSED\n";
            other_button->setStyleSheet("background-color:blue");
        }
    };
    auto released_func = [&other_button](){
        if(!other_button->isCheckable()) {
            std::cout << "RELEASED\n";
            other_button->setStyleSheet("background-color:white");
        }
    };
    auto toggle_func = [&other_button](){
        if(other_button->isChecked()) {
            std::cout << "PRESSED\n";
            other_button->setStyleSheet("background-color:red");
        }
        else {
            std::cout << "RELEASED\n";
            other_button->setStyleSheet("background-color:white");
        }
    };
    QPushButton::connect(other_button,&QPushButton::pressed,other_window,pressed_func);
    QPushButton::connect(other_button,&QPushButton::released,other_window,released_func);
    QPushButton::connect(other_button,&QPushButton::toggled,other_window,toggle_func);

    auto* checkbox = new QCheckBox();
    auto toggle_button = [&other_button, &checkbox](){
        other_button->setCheckable(checkbox->isChecked());
    };
    QCheckBox::connect(checkbox,&QCheckBox::stateChanged,other_window,toggle_button);
    auto* other_grid = new QGridLayout();

    other_grid->addWidget(other_button,0,0);
    other_grid->addWidget(checkbox,0,1);
    other_window->setLayout(other_grid);

    other_window->show();
    return app.exec();
}