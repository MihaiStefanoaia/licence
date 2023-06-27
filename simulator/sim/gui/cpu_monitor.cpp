//
// Created by mihai-laptop on 6/27/23.
//

#include "cpu_monitor.h"

namespace sim {
    namespace gui {
        cpu_monitor::cpu_monitor(objs::cpu &processor) : processor(processor){
            window = new QWidget();
            layout_main = new QGridLayout(window);

            rga_lbl = new QLabel("rga");
            rgb_lbl = new QLabel("rgb");
            rgc_lbl = new QLabel("rgc");
            rgd_lbl = new QLabel("rgd");

            rmx_lbl = new QLabel("rmx");
            rsx_lbl = new QLabel("rsx");
            rbx_lbl = new QLabel("rbx");
            rpx_lbl = new QLabel("rpx");

            rgf_lbl = new QLabel("rgf");
            rgi_lbl = new QLabel("rgi");

            state_lbl = new QLabel("state:");
            err_lbl = new QLabel("error:");

            rga_val = new QLabel("00");
            rgb_val = new QLabel("00");
            rgc_val = new QLabel("00");
            rgd_val = new QLabel("00");

            rmx_val = new QLabel("0000");
            rsx_val = new QLabel("0000");
            rbx_val = new QLabel("0000");
            rpx_val = new QLabel("0000");

            rgf_val = new QLabel("0");
            rgi_val = new QLabel("0000_0000");

            state_val = new QLabel();
            err_val = new QLabel();

            layout_8b = new QGridLayout(window);
            layout_8b->addWidget(rga_lbl,0,0,Qt::AlignLeft);
            layout_8b->addWidget(rgb_lbl,1,0,Qt::AlignLeft);
            layout_8b->addWidget(rgc_lbl,2,0,Qt::AlignLeft);
            layout_8b->addWidget(rgd_lbl,3,0,Qt::AlignLeft);

            layout_8b->addWidget(rga_val,0,1,Qt::AlignRight);
            layout_8b->addWidget(rgb_val,1,1,Qt::AlignRight);
            layout_8b->addWidget(rgc_val,2,1,Qt::AlignRight);
            layout_8b->addWidget(rgd_val,3,1,Qt::AlignRight);

            layout_16b = new QGridLayout();
            layout_16b->addWidget(rmx_lbl,0,2,Qt::AlignLeft);
            layout_16b->addWidget(rsx_lbl,1,2,Qt::AlignLeft);
            layout_16b->addWidget(rbx_lbl,2,2,Qt::AlignLeft);
            layout_16b->addWidget(rpx_lbl,3,2,Qt::AlignLeft);

            layout_16b->addWidget(rmx_val,0,3,Qt::AlignRight);
            layout_16b->addWidget(rsx_val,1,3,Qt::AlignRight);
            layout_16b->addWidget(rbx_val,2,3,Qt::AlignRight);
            layout_16b->addWidget(rpx_val,3,3,Qt::AlignRight);

            layout_main->addLayout(layout_8b,0,0);
            layout_main->addLayout(layout_16b,0,1);

            layout_main->addWidget(rgf_lbl,1,0,Qt::AlignLeft);
            layout_main->addWidget(rgf_val,1,1,Qt::AlignRight);

            layout_main->addWidget(rgi_lbl,2,0,Qt::AlignLeft);
            layout_main->addWidget(rgi_val,2,1,Qt::AlignRight);

            layout_main->addWidget(state_lbl,3,0,Qt::AlignLeft);
            layout_main->addWidget(state_val,3,1,Qt::AlignRight);

            layout_main->addWidget(err_lbl,4,0,Qt::AlignLeft);
            layout_main->addWidget(err_val,4,1,Qt::AlignRight);

            window->setLayout(layout_main);
        }

        void cpu_monitor::update() {
            basic_output::update();
        }

        void cpu_monitor::render() {
            rga_val->setText(std::to_string(processor.rga).c_str());
            rgb_val->setText(std::to_string(processor.rgb).c_str());
            rgc_val->setText(std::to_string(processor.rgc).c_str());
            rgd_val->setText(std::to_string(processor.rgd).c_str());

            rmx_val->setText(std::to_string(processor.rmx).c_str());
            rsx_val->setText(std::to_string(processor.rsx).c_str());
            rbx_val->setText(std::to_string(processor.rbx).c_str());
            rpx_val->setText(std::to_string(processor.rpx).c_str());

            rgi_val->setText(std::to_string(processor.rgi).c_str());
            rgf_val->setText(std::to_string(processor.rgf).c_str());

            state_val->setText(processor.state_str.at(processor.state).c_str());
            err_val->setText(processor.err_str.at(processor.err_state).c_str());
        }
    } // sim
} // gui