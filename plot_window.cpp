#include "plot_window.h"
#include <QPushButton>
#include "qcustomplot.h"

PlotWindow::PlotWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //win size
    setFixedSize(xDim, yDim);
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-xWidth-padX, yDim-yHeight-padY/2, xWidth, yHeight);
    connect(close_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));
    //save button

    //persistence checkbox

    //plot section
    qplot = new QCustomPlot(this);
    qplot->setGeometry(xDim/4, padY, xDim*3/4-padX, yDim-yHeight-2*padY);
    qplot->replot();
}

void PlotWindow::SetDimensions()
{
    xDim = 800;
    yDim = 500;
    xWidth = 80;
    yHeight = 30;
    padX = 10;
    padY = 10;
}
