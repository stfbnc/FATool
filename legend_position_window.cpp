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
    ok_button->setEnabled(false);
    connect(ok_button, &QPushButton::clicked, [=](){this->onOKClick(plt);});
    //apply button
    apply_button = new QPushButton("Apply", this);
    apply_button->setGeometry(xDim-padX-xWidth*6, yDim-yHeight-padY, xWidth*3, yHeight);
    apply_button->setEnabled(false);
    connect(apply_button, &QPushButton::clicked, [=](){this->RefreshLegend(plt);});
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-padX*3/2-xWidth*9, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
    //checkboxes
    num_cbs = 5;
    cb_selected = -1;
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
    xDim = 360;
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
    if(idx == cb_selected && !cbs[idx]->isChecked()){
        coordTxt->setEnabled(false);
        ok_button->setEnabled(false);
        apply_button->setEnabled(false);
    }else{
        cb_selected = idx;
        ok_button->setEnabled(true);
        apply_button->setEnabled(true);
    }
}

void MoveLegendWindow::RefreshLegend(QCustomPlot *plt)
{
    switch(cb_selected){
        case 0:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
            break;
        case 1:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignTop);
            break;
        case 2:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
            break;
        case 3:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
            break;
        case 4:
            QStringList xl = coordTxt->text().replace(QRegExp("\\s+"), "").split(",");
            QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
            if(xl.size() != 2 || (!xl.first().contains(rgx) || !xl.last().contains(rgx))){
                refreshOK = false;
                QMessageBox messageBox;
                QString errToShow = "Input must be numeric and\nmust contain two values!";
                messageBox.critical(nullptr, "Error", errToShow);
                messageBox.setFixedSize(200,200);
            }else{
                refreshOK = true;
                QFont qFont = QFont(font().family(), 12);
                plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
                QFontMetrics fm(qFont);
                int num_graphs = plt->itemCount();
                int pixelsWide = 0;
                for(int i = 0; i < num_graphs; i++){
                    int w = fm.width(plt->graph(i)->name());
                    if(w > pixelsWide)
                        pixelsWide = w;
                }
                int pixelsHigh = fm.height() * num_graphs;
                double xPad = plt->xAxis->pixelToCoord(23) - plt->xAxis->pixelToCoord(10);
                double yPad = plt->yAxis->pixelToCoord(21) - plt->yAxis->pixelToCoord(10);
                double xMin = plt->xAxis->range().lower + xPad;
                double xMax = plt->xAxis->range().upper - xPad;
                double yMin = plt->yAxis->range().lower - yPad;
                double yMax = plt->yAxis->range().upper + yPad;
                double xRange = xMax - xMin;
                double yRange = yMin - yMax;
                QPointF crd((coordTxt->text().split(",").first().trimmed().toDouble()-xMin)/static_cast<double>(xRange),
                            (coordTxt->text().split(",").last().trimmed().toDouble()-yMax)/static_cast<double>(yRange));
                QRectF rect = plt->axisRect()->insetLayout()->insetRect(0);
                rect.moveTopLeft(crd);
                rect.setWidth(pixelsWide);
                rect.setHeight(pixelsHigh);
                plt->axisRect()->insetLayout()->setInsetRect(0, rect);
            }
            break;
    }
    plt->replot();
}

void MoveLegendWindow::onOKClick(QCustomPlot *plt)
{
    RefreshLegend(plt);
    if(refreshOK)
        close();
}
