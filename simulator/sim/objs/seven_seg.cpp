//
// Created by mihai-pc on 6/16/23.
//

#include <QPainter>
#include <QGraphicsView>
#include "seven_seg.h"

namespace sim {
    namespace objs {
        seven_seg::seven_seg(const bit_array &arr, bit& en) : input(arr), enable(en) {
            window = new QWidget();
            graphics_scene = new QGraphicsScene(0,0,50,90,window);
            view = new QGraphicsView(graphics_scene,window);
            segments[SEG_A] = QPolygon();
            segments[SEG_A].putPoints(0,4,0,0,45,0,45,5,0,5);
        }

        void seven_seg::update() {

        }

        void seven_seg::render() {
            QPainter painter(view);
            QBrush brush;
            brush.setColor(Qt::green);
            brush.setStyle(Qt::SolidPattern);

            // Fill polygon
            QPainterPath path;
            path.addPolygon(segments[SEG_A]);

            // Draw polygon
            painter.drawPolygon(segments[SEG_A]);
            painter.fillPath(path, brush);
        }
    } // sim
} // objs