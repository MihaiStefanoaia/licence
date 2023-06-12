//
// Created by mihai-pc on 6/11/23.
//

#ifndef SIMULATOR_MONITORED_H
#define SIMULATOR_MONITORED_H

#include <QWidget>

namespace sim {

    class monitored {
    protected:
        QWidget* window;
    public:
        QWidget* get_window(){
            return window;
        };
    };

} // sim

#endif //SIMULATOR_MONITORED_H
