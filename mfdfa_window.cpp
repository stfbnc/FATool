#include "mfdfa_window.h"

MFDFAWindow::MFDFAWindow(MFDFA *mfdfa_, QWidget *parent) : PlotWindow(parent)
{
    mfdfa = mfdfa_;
    //set title
    QString winTitle = strMFDFA+" - "+QString::fromStdString(mfdfa->getFileName()).split("/").last();
    setTitle(winTitle);
    //plot
    plotData();
    //plot fields
    addPlotFields();
    //legend
    addLegend();
}

MFDFAWindow::~MFDFAWindow(){}

void MFDFAWindow::plotData()
{
    mfdfa->plot(plt);
    plt->replot();
}

void MFDFAWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        mfdfa->saveFile(pathToSave.toStdString());
}
