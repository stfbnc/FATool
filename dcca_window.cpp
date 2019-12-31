#include "dcca_window.h"

DCCAWindow::DCCAWindow(DCCA *dcca_, QWidget *parent) : PlotWindow(parent)
{
    dcca = dcca_;
    //set title
    QString winTitle = strDCCA+" - "+QString::fromStdString(dcca->getFileName1()).split("/").last();
    winTitle.append(" & "+QString::fromStdString(dcca->getFileName2()).split("/").last());
    setTitle(winTitle);
    //refit button
    addRefitButton();
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
