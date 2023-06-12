//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_BASIC_IO_H
#define SIMULATOR_BASIC_IO_H

#include <QWidget>
#include "monitored.h"

namespace sim {

    class basic_io : public monitored{
    protected:
        bool in_another_layout = false;
        bool flagged = false;
    public:
        basic_io() : monitored() {};
        virtual ~basic_io() = default;
        virtual void update() = 0;
        friend class evaluation_list;
    };


} // sim

#endif //SIMULATOR_BASIC_IO_H
