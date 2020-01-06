#include "dfa_window.h"

DFAWindow::DFAWindow(DFA *dfa_, QWidget *parent) : PlotWindow(parent)
{
    dfa = dfa_;
    fileName = QString::fromStdString(dfa->getFileName()).split("/").last();
    //set title
    QString winTitle = strDFA+" - "+fileName;
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

DFAWindow::~DFAWindow(){}

void DFAWindow::plotData()
{
    dfa->plot(plt);
    plt->setLogAxes(BasePlot::XY);
    plt->replot();
}

void DFAWindow::refitData(int start, int end, double *hSlope, double *hIntcpt)
{
    dfa->fitFlucVec(start, end);
    *hIntcpt = dfa->getHintercept();
    *hSlope = dfa->getH();
}

void DFAWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        dfa->saveFile(pathToSave.toStdString());
}
