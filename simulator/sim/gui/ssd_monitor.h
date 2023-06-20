//
// Created by mihai-pc on 6/16/23.
//

#ifndef SIMULATOR_SSD_MONITOR_H
#define SIMULATOR_SSD_MONITOR_H

#include <QPainter>
#include <QWidget>

namespace sim {
    namespace gui {


        class ssd_monitor : public QWidget {
            QColor segment_colors[8] = {Qt::black, Qt::black, Qt::black, Qt::black,
                                        Qt::black, Qt::black, Qt::black, Qt::black};
        public:
            explicit ssd_monitor(QWidget *parent = nullptr);

            void set_segment_color(int segment, const QColor &color);
        protected:
            void paintEvent(QPaintEvent *) override;
        };

    } // sim
} // gui

#endif //SIMULATOR_SSD_MONITOR_H
