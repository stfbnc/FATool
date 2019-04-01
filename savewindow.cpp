#include "savewindow.h"

SaveWindow::SaveWindow(QCustomPlot *plt, QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle("Save plot");
    //win size
    setFixedSize(xDim, yDim);
    //save button
    save_button = new QPushButton("Save", this);
    save_button->setGeometry(xDim-(padX/2+xWidth*3/4), yDim-yHeight-padY, xWidth*3/4, yHeight);
    //connect(save_button, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(save_button, &QPushButton::clicked, [=](){this->onSave(plt);});
    //apply button
    apply_button = new QPushButton("Apply", this);
    apply_button->setGeometry(xDim-2*(padX/2+xWidth*3/4), yDim-yHeight-padY, xWidth*3/4, yHeight);
    connect(apply_button, &QPushButton::clicked, [=](){this->onApply(plt);});
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-3*(padX/2+xWidth*3/4), yDim-yHeight-padY, xWidth*3/4, yHeight);
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
    //ranges
    xlim = new QLabel("x limits\n(comma-separated)", this);
    xlim->setGeometry(padX, padY, xWidth, yHeight);
    xlimTxt = new QTextEdit(this);
    xlimTxt->setGeometry(2*padX+xWidth, padY, 2*xWidth, yHeight);
    ylim = new QLabel("y limits\n(comma-separated)", this);
    ylim->setGeometry(padX, 2*padY+yHeight, xWidth, yHeight);
    ylimTxt = new QTextEdit(this);
    ylimTxt->setGeometry(2*padX+xWidth, 2*padY+yHeight, 2*xWidth, yHeight);
    //labels
    title = new QLabel("Title", this);
    title->setGeometry(padX, 3*padY+2*yHeight, xWidth, yHeight);
    titleTxt = new QTextEdit(this);
    titleTxt->setGeometry(2*padX+xWidth, 3*padY+2*yHeight, 2*xWidth, yHeight);
    xlabel = new QLabel("x label", this);
    xlabel->setGeometry(padX, 4*padY+3*yHeight, xWidth, yHeight);
    xlabelTxt = new QTextEdit(this);
    xlabelTxt->setGeometry(2*padX+xWidth, 4*padY+3*yHeight, 2*xWidth, yHeight);
    ylabel = new QLabel("y label", this);
    ylabel->setGeometry(padX, 5*padY+4*yHeight, xWidth, yHeight);
    ylabelTxt = new QTextEdit(this);
    ylabelTxt->setGeometry(2*padX+xWidth, 5*padY+4*yHeight, 2*xWidth, yHeight);
    //legend
    legend = new QLabel("Legend\n(semicolon-separated)", this);
    legend->setGeometry(padX, 6*padY+5*yHeight, xWidth, yHeight);
    legendTxt = new QTextEdit(this);
    legendTxt->setGeometry(2*padX+xWidth, 6*padY+5*yHeight, 2*xWidth, 3*yHeight);
}

SaveWindow::~SaveWindow(){}

void SaveWindow::SetDimensions(){
    xDim = 450;
    yDim = 350;
    xWidth = 140;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void SaveWindow::onApply(QCustomPlot *plt){
    QStringList xl = xlimTxt->toPlainText().split(",");
    QStringList yl = ylimTxt->toPlainText().split(",");
    QStringList lg = legendTxt->toPlainText().split(";");
    QStringList save_alert;
    if(xl.size() != 2)
        save_alert.append("- x limits");
    if(yl.size() != 2)
        save_alert.append("- y limits");
    if(lg.size() != plt->legend->itemCount())
        save_alert.append("- legend");
    if(save_alert.size() > 0)
        plt->xAxis->setRange(xl.first().trimmed().toDouble(), xl.last().trimmed().toDouble());
        plt->yAxis->setRange(yl.first().trimmed().toDouble(), yl.last().trimmed().toDouble());
        plt->xAxis->setLabel(xlabelTxt->toPlainText().trimmed());
        plt->yAxis->setLabel(ylabelTxt->toPlainText().trimmed());
        plt->xAxis2->setLabel(titleTxt->toPlainText().trimmed());
        for(int i = 0; i < plt->legend->itemCount(); i++)
            plt->graph(i)->setName(lg[i].trimmed());
        plt->replot();
    //else
        //messaggio di errore con la lista
}

void SaveWindow::onSave(QCustomPlot *plt){
    QString save_file;
    QFileDialog save_dialog;
    save_file = save_dialog.getSaveFileName();
    plt->savePdf(save_file);
}
