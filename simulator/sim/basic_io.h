//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_BASIC_IO_H
#define SIMULATOR_BASIC_IO_H

#include <QWidget>

namespace sim {

    class basic_io {
    protected:
        QWidget* window = nullptr;
        bool flagged = false;
    public:
        virtual ~basic_io() = default;
        virtual void update() = 0;
        friend class evaluation_list;
        QWidget* get_window(){
            return window;
        };
    };


} // sim

#endif //SIMULATOR_BASIC_IO_H
