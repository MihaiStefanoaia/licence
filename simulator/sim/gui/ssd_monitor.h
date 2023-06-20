//
// Created by mihai-pc on 6/16/23.
//

#ifndef SIMULATOR_SSD_MONITOR_H
#define SIMULATOR_SSD_MONITOR_H

namespace sim {
    namespace objs{
        class seven_seg;
    }
    namespace gui {

        class ssd_monitor {
            objs::seven_seg* ssd;
        };

    } // sim
} // gui

#endif //SIMULATOR_SSD_MONITOR_H
