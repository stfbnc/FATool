#include "main_window.h"
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle("FATool");
    //win size
    setFixedSize(xDim, yDim);
    //load button
    load_button = new QPushButton("Load file(s)", this);
    load_button->setGeometry(padX, padY/2, xWidth, yHeight);
    connect(load_button, SIGNAL (clicked()), QApplication::instance(), SLOT (onLoadClick()));
    //save button
    save_button = new QPushButton("Save plot", this);
    save_button->setGeometry(2*padX+xWidth, padY/2, xWidth, yHeight);
    //text
    fluct_txt = new QLabel("Type of analysis:", this);
    fluct_txt->setGeometry((xDim+padX+xWidth)/2-padX-xWidth*3/2, padY/2, xWidth, yHeight);
    fluct_txt->setStyleSheet("font-weight: bold");
    //dropdown list
    dd_list = new QComboBox(this);
    FillList();
    dd_list->setGeometry((xDim+padX)/2+1, padY/2+2, xWidth, yHeight);
    //go button
    go_button = new QPushButton("Go!", this);
    go_button->setGeometry((xDim+padX+xWidth)/2+padX+xWidth/2, padY/2, xWidth, yHeight);
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-xWidth-padX, padY/2, xWidth, yHeight);
    connect(close_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));
    //plot section
    qplot = new QCustomPlot(this);
    qplot->setGeometry(padX, padY+yHeight, xDim-2*padX, yDim-yHeight-2*padY);
    qplot->xAxis->setLabel("time");
    qplot->yAxis->setLabel("Time series");
    qplot->replot();
}

void MainWindow::SetDimensions()
{
    xDim = 800;
    yDim = 500;
    xWidth = 110;
    yHeight = 30;
    padX = 10;
    padY = 10;
}

void MainWindow::FillList()
{
    dd_list->addItem("-");
    dd_list->addItem("DFA");
    dd_list->addItem("MFDFA");
    dd_list->addItem("DCCA");
    dd_list->addItem("Ht");
    dd_list->addItem("rhoDCCA");
}

void MainWindow::onLoadClick()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
}
