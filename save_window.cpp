#include "save_window.h"

SaveWindow::SaveWindow(QCustomPlot *plt, QWidget *parent) : QWidget(parent)
{
    //set dimensions
    setDimensions();
    //set title
    setWindowTitle("Save plot");
    //win size
    setFixedSize(xDim, yDim);
    //save button
    saveButton = new QPushButton("Save", this);
    saveButton->setGeometry(xDim-(padX/2+xWidth*3/4), yDim-yHeight-padY, xWidth*3/4, yHeight);
    connect(saveButton, &QPushButton::clicked, [=](){this->onSave(plt);});
    //apply button
    applyButton = new QPushButton("Apply", this);
    applyButton->setGeometry(xDim-2*(padX/2+xWidth*3/4), yDim-yHeight-padY, xWidth*3/4, yHeight);
    connect(applyButton, &QPushButton::clicked, [=](){this->onApply(plt);});
    //close button
    closeButton = new QPushButton("Close", this);
    closeButton->setGeometry(xDim-3*(padX/2+xWidth*3/4), yDim-yHeight-padY, xWidth*3/4, yHeight);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    //ranges
    xLimLbl = new QLabel("x limits\n(comma-separated)", this);
    xLimLbl->setGeometry(padX, padY, xWidth, yHeight);
    xLimTxt = new QLineEdit(QString::number(plt->xAxis->range().lower)+","+QString::number(plt->xAxis->range().upper), this);
    xLimTxt->setGeometry(2*padX+xWidth, padY, 2*xWidth, yHeight);
    yLimLbl = new QLabel("y limits\n(comma-separated)", this);
    yLimLbl->setGeometry(padX, 2*padY+yHeight, xWidth, yHeight);
    yLimTxt = new QLineEdit(QString::number(plt->yAxis->range().lower)+","+QString::number(plt->yAxis->range().upper), this);
    yLimTxt->setGeometry(2*padX+xWidth, 2*padY+yHeight, 2*xWidth, yHeight);
    //labels
    titleLbl = new QLabel("Title", this);
    titleLbl->setGeometry(padX, 3*padY+2*yHeight, xWidth, yHeight);
    titleTxt = new QLineEdit(plt->xAxis2->label(), this);
    titleTxt->setGeometry(2*padX+xWidth, 3*padY+2*yHeight, 2*xWidth, yHeight);
    xLabelLbl = new QLabel("x label", this);
    xLabelLbl->setGeometry(padX, 4*padY+3*yHeight, xWidth, yHeight);
    xLabelTxt = new QLineEdit(plt->xAxis->label(), this);
    xLabelTxt->setGeometry(2*padX+xWidth, 4*padY+3*yHeight, 2*xWidth, yHeight);
    yLabelLbl = new QLabel("y label", this);
    yLabelLbl->setGeometry(padX, 5*padY+4*yHeight, xWidth, yHeight);
    yLabelTxt = new QLineEdit(plt->yAxis->label(), this);
    yLabelTxt->setGeometry(2*padX+xWidth, 5*padY+4*yHeight, 2*xWidth, yHeight);
    //legend
    legendLbl = new QLabel("Legend\n(semicolon-separated)", this);
    legendLbl->setGeometry(padX, 6*padY+5*yHeight, xWidth, yHeight);
    QString lgnd = plt->graph(0)->name();
    for(int i = 1; i < plt->legend->itemCount(); i++)
        lgnd += ";"+plt->graph(i)->name();
    legendTxt = new QTextEdit(lgnd, this);
    legendTxt->setTabChangesFocus(true);
    legendTxt->setGeometry(2*padX+xWidth, 6*padY+5*yHeight, 2*xWidth, 3*yHeight);
}

SaveWindow::~SaveWindow(){}

void SaveWindow::setDimensions()
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
    QStringList xl = xLimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList yl = yLimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList lg = legendTxt->toPlainText().split(";");
    QStringList saveAlert;
    QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
    if(xl.size() != 2 ||
       (!xl.first().contains(rgx) || !xl.last().contains(rgx)))
        saveAlert.append("\n- x limits");
    if(yl.size() != 2 ||
       (!yl.first().contains(rgx) || !yl.last().contains(rgx)))
        saveAlert.append("\n- y limits");
    if(lg.size() != plt->legend->itemCount() || (lg.size() == 1 && lg.first() == ""))
        saveAlert.append("\n- legend");
    if(saveAlert.size() == 0){
        plt->xAxis->setRange(xl.first().trimmed().toDouble(), xl.last().trimmed().toDouble());
        plt->yAxis->setRange(yl.first().trimmed().toDouble(), yl.last().trimmed().toDouble());
        plt->xAxis->setLabel(xLabelTxt->text().trimmed());
        plt->yAxis->setLabel(yLabelTxt->text().trimmed());
        plt->xAxis2->setLabel(titleTxt->text().trimmed());
        for(int i = 0; i < plt->legend->itemCount(); i++)
            plt->graph(i)->setName(lg[i].trimmed());
        plt->replot();
    }else{
        QMessageBox messageBox;
        QString errToShow = "An error occured in\n the following fields:\n";
        for(int i = 0; i < saveAlert.size(); i++)
            errToShow.append(saveAlert[i]);
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
}

void SaveWindow::onSave(QCustomPlot *plt)
{
    QString saveFile;
    QFileDialog saveDialog;
    saveFile = saveDialog.getSaveFileName();
    QString fileExt = saveFile.split(".").last();
    if(!fileExt.isNull() && !fileExt.isEmpty()){
        if(fileExt == "pdf")
            plt->savePdf(saveFile);
        else if(fileExt == "png")
            plt->savePng(saveFile);
        else{
            QMessageBox messageBox;
            QString errToShow = "Supported formats are:\n- pdf\n- png";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
    }
}
