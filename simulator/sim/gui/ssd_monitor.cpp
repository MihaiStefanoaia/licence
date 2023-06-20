//
// Created by mihai-pc on 6/16/23.
//

#include "ssd_monitor.h"

namespace sim {
    namespace gui {
        void ssd_monitor::paintEvent(QPaintEvent *)  {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(Qt::NoPen);

            // Draw segments
            for (int i = 0; i < 8; ++i) {
                painter.setBrush(segment_colors[i]);

                if (i == 0) // Segment A
                    painter.drawRect(5, 0, 20, 5);
                else if (i == 1) // Segment B
                    painter.drawRect(25, 5, 5, 20);
                else if (i == 2) // Segment C
                    painter.drawRect(25, 30, 5, 20);
                else if (i == 3) // Segment D
                    painter.drawRect(5, 50, 20, 5);
                else if (i == 4) // Segment E
                    painter.drawRect(0, 30, 5, 20);
                else if (i == 5) // Segment F
                    painter.drawRect(0, 5, 5, 20);
                else if (i == 6) // Segment G
                    painter.drawRect(5, 25, 20, 5);
                else // segment H
                    painter.drawEllipse(30, 50, 5, 5);
            }

            // Draw dot segment
        }

        void ssd_monitor::set_segment_color(int segment, const QColor &color) {
            segment_colors[segment] = color;
            update();
        }

        ssd_monitor::ssd_monitor(QWidget *parent) : QWidget(parent) {
            setFixedSize(35, 55);
        }    } // sim
} // gui