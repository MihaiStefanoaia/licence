//
// Created by Mihai - PC on 11/24/2022.
//

#ifndef SIMULATOR_BOOTLOADER_H
#define SIMULATOR_BOOTLOADER_H

#include "bit.h"
#include "basic_io.h"
#include "memory.h"
#include <string>
#include <QPushButton>
#include <QLabel>

namespace sim {
    namespace gui {

        class bootloader : public basic_io {
            QPushButton* load;
            QLabel* lbl;
            objs::memory& mem;
        public:
            bootloader(std::string name, objs::memory& mem);
        };

    } // sim
} // objs

#endif //SIMULATOR_BOOTLOADER_H
