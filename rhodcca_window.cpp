#include "rhodcca_window.h"

rhoDCCAWindow::rhoDCCAWindow(rhoDCCA *rhodcca_, QWidget *parent) : PlotWindow(parent)
{
    rhodcca = rhodcca_;
    //set title
    QString winTitle = strRHODCCA+" - "+QString::fromStdString(rhodcca->getFileName1()).split("/").last();;
    winTitle.append(" & "+QString::fromStdString(rhodcca->getFileName2()).split("/").last());
    setTitle(winTitle);
    //plot
    plotData();
    //plot fields
    addPlotFields();
    //legend
    addLegend();
}

rhoDCCAWindow::~rhoDCCAWindow(){}

void rhoDCCAWindow::plotData()
{
    rhodcca->plot(plt);
    plt->replot();
}

void rhoDCCAWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        rhodcca->saveFile(pathToSave.toStdString());
}
