#include "main_window.h"
#include <QPushButton>
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //win size
    setFixedSize(xDim, yDim);
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-xWidth-padX, yDim-yHeight-padY/2, xWidth, yHeight);
    connect(close_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));
    //load button
    load_button = new QPushButton("Load file(s)", this);
    load_button->setGeometry(padX, padY, xWidth*4/3, yHeight);
    //persistence checkbox

    //plot section
    qplot = new QCustomPlot(this);
    qplot->setGeometry(xDim/6, padY, xDim*5/6-padX, yDim-yHeight-2*padY);
    qplot->replot();
}

void MainWindow::SetDimensions()
{
    xDim = 1000;
    yDim = 500;
    xWidth = 80;
    yHeight = 30;
    padX = 10;
    padY = 10;
}
