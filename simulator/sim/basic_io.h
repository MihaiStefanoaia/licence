//
// Created by mihai-pc on 4/1/23.
//

#ifndef SIMULATOR_BASIC_IO_H
#define SIMULATOR_BASIC_IO_H

namespace sim {

    class basic_io {
    protected:
        bool flagged = false;
    public:
        virtual void update() = 0;
        friend class evaluation_list;
    };

} // sim

#endif //SIMULATOR_BASIC_IO_H
