#include "dcca_window.h"

DCCAWindow::DCCAWindow(DCCA *dcca_, QWidget *parent) : PlotWindow(parent)
{
    dcca = dcca_;
    fileName = QString::fromStdString(dcca->getFileName1()).split("/").last();
    fileName2 = QString::fromStdString(dcca->getFileName2()).split("/").last();
    //set title
    QString winTitle = strDCCA+" - "+fileName;
    winTitle.append(" & "+fileName2);
    setTitle(winTitle);
    //refit button
    addRefitButton();
    //fits log button
    addFitLogButton();
    //plot
    plotData();
    //plot fields
    addPlotFields();
    //legend
    addLegend();
}

DCCAWindow::~DCCAWindow(){}

void DCCAWindow::plotData()
{
    dcca->plot(plt);
    plt->setLogAxes(BasePlot::XY);
    plt->replot();
}

void DCCAWindow::refitData(int start, int end, double *hSlope, double *hIntcpt)
{
    dcca->fitFlucVec(start, end);
    *hIntcpt = dcca->getHintercept();
    *hSlope = dcca->getH();
}

void DCCAWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        dcca->saveFile(pathToSave.toStdString());
}
