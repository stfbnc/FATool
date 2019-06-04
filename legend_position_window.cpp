#include "legend_position_window.h"

MoveLegendWindow::MoveLegendWindow(QCustomPlot *plt, QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle("Legend position");
    //win size
    setFixedSize(xDim, yDim);
    //ok button
    ok_button = new QPushButton("OK", this);
    ok_button->setGeometry(xDim-padX/2-xWidth*3, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(ok_button, &QPushButton::clicked, [=](){this->onOKClick(plt);});
    //apply button
    apply_button = new QPushButton("Apply", this);
    apply_button->setGeometry(xDim-padX-xWidth*6, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(apply_button, &QPushButton::clicked, [=](){this->RefreshLegend(plt);});
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-padX*3/2-xWidth*9, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
    //checkboxes
    num_cbs = 5;
    labels.append({"Top left", "Top right", "Bottom left", "Bottom right", "Coordinates (comma-separated)"});
    cbs = new QCheckBox* [num_cbs];
    for(int i = 0; i < num_cbs; i++){
        cbs[i] = new QCheckBox(labels[i], this);
        cbs[i]->setGeometry(padX, padY+yHeight*i, xWidth*8, yHeight);
        connect(cbs[i], &QCheckBox::clicked, [=](){this->onCheckBoxClick(i);});
    }
    coordTxt = new QLineEdit(this);
    coordTxt->setGeometry(padX+xWidth*8, padY+yHeight*4, xWidth*3, yHeight);
    coordTxt->setEnabled(false);
}

MoveLegendWindow::~MoveLegendWindow(){}

void MoveLegendWindow::SetDimensions()
{
    xDim = 400;
    yDim = 200;
    xWidth = 30;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void MoveLegendWindow::onCheckBoxClick(int idx)
{
    for(int i = 0; i < num_cbs; i++){
        if(i != idx)
            cbs[i]->setChecked(false);
    }
    if(idx != num_cbs-1)
        coordTxt->setEnabled(false);
    else
        coordTxt->setEnabled(true);
    cb_selected = idx;
}

void MoveLegendWindow::RefreshLegend(QCustomPlot *plt)
{
    switch(cb_selected){
        case 0:
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
            break;
        case 1:
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignTop);
            break;
        case 2:
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
            break;
        case 3:
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
            break;
        case 4:
            QRectF rect = plt->axisRect()->insetLayout()->insetRect(0);
            QPointF crd((coordTxt->text().split(",").first().toDouble()-plt->axisRect()->left())/static_cast<double>(plt->axisRect()->width()),
                        (coordTxt->text().split(",").last().toDouble()-plt->axisRect()->top())/static_cast<double>(plt->axisRect()->height()));
            qInfo() << crd;
            rect.moveTopLeft(crd);
            plt->axisRect()->insetLayout()->setInsetRect(0, rect);
            break;
    }
    plt->replot();
}

void MoveLegendWindow::onOKClick(QCustomPlot *plt)
{
    RefreshLegend(plt);
    close();
}
