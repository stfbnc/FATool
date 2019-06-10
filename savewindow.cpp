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
    xlimTxt = new QLineEdit(QString::number(plt->xAxis->range().lower)+","+QString::number(plt->xAxis->range().upper), this);
    xlimTxt->setGeometry(2*padX+xWidth, padY, 2*xWidth, yHeight);
    ylim = new QLabel("y limits\n(comma-separated)", this);
    ylim->setGeometry(padX, 2*padY+yHeight, xWidth, yHeight);
    ylimTxt = new QLineEdit(QString::number(plt->yAxis->range().lower)+","+QString::number(plt->yAxis->range().upper), this);
    ylimTxt->setGeometry(2*padX+xWidth, 2*padY+yHeight, 2*xWidth, yHeight);
    //labels
    title = new QLabel("Title", this);
    title->setGeometry(padX, 3*padY+2*yHeight, xWidth, yHeight);
    titleTxt = new QLineEdit(plt->xAxis2->label(), this);
    titleTxt->setGeometry(2*padX+xWidth, 3*padY+2*yHeight, 2*xWidth, yHeight);
    xlabel = new QLabel("x label", this);
    xlabel->setGeometry(padX, 4*padY+3*yHeight, xWidth, yHeight);
    xlabelTxt = new QLineEdit(plt->xAxis->label(), this);
    xlabelTxt->setGeometry(2*padX+xWidth, 4*padY+3*yHeight, 2*xWidth, yHeight);
    ylabel = new QLabel("y label", this);
    ylabel->setGeometry(padX, 5*padY+4*yHeight, xWidth, yHeight);
    ylabelTxt = new QLineEdit(plt->yAxis->label(), this);
    ylabelTxt->setGeometry(2*padX+xWidth, 5*padY+4*yHeight, 2*xWidth, yHeight);
    //legend
    legend = new QLabel("Legend\n(semicolon-separated)", this);
    legend->setGeometry(padX, 6*padY+5*yHeight, xWidth, yHeight);
    QString lgnd = plt->graph(0)->name();
    for(int i = 1; i < plt->legend->itemCount(); i++)
        lgnd += ";"+plt->graph(i)->name();
    legendTxt = new QTextEdit(lgnd, this);
    legendTxt->setTabChangesFocus(true);
    legendTxt->setGeometry(2*padX+xWidth, 6*padY+5*yHeight, 2*xWidth, 3*yHeight);
}

SaveWindow::~SaveWindow(){}

void SaveWindow::SetDimensions()
{
    xDim = 450;
    yDim = 350;
    xWidth = 140;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void SaveWindow::onApply(QCustomPlot *plt)
{
    QStringList xl = xlimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList yl = ylimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList lg = legendTxt->toPlainText().split(";");
    QStringList save_alert;
    QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
    if(xl.size() != 2 ||
       (!xl.first().contains(rgx) || !xl.last().contains(rgx)))
        save_alert.append("\n- x limits");
    if(yl.size() != 2 ||
       (!yl.first().contains(rgx) || !yl.last().contains(rgx)))
        save_alert.append("\n- y limits");
    if(lg.size() != plt->legend->itemCount() || (lg.size() == 1 && lg.first() == ""))
        save_alert.append("\n- legend");
    if(save_alert.size() == 0){
        plt->xAxis->setRange(xl.first().trimmed().toDouble(), xl.last().trimmed().toDouble());
        plt->yAxis->setRange(yl.first().trimmed().toDouble(), yl.last().trimmed().toDouble());
        plt->xAxis->setLabel(xlabelTxt->text().trimmed());
        plt->yAxis->setLabel(ylabelTxt->text().trimmed());
        plt->xAxis2->setLabel(titleTxt->text().trimmed());
        for(int i = 0; i < plt->legend->itemCount(); i++)
            plt->graph(i)->setName(lg[i].trimmed());
        plt->replot();
    }else{
        QMessageBox messageBox;
        QString errToShow = "An error occured in\n the following fields:\n";
        for(int i = 0; i < save_alert.size(); i++)
            errToShow.append(save_alert[i]);
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(200,200);
    }
}

void SaveWindow::onSave(QCustomPlot *plt)
{
    QString save_file;
    QFileDialog save_dialog;
    save_file = save_dialog.getSaveFileName();
    QString file_ext = save_file.split(".").last();
    if(!file_ext.isNull() && !file_ext.isEmpty()){
        if(file_ext == "pdf")
            plt->savePdf(save_file);
        else if(file_ext == "png")
            plt->savePng(save_file);
        else{
            QMessageBox messageBox;
            QString errToShow = "Supported formats are:\n- pdf\n- png";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(200,200);
        }
    }
}
