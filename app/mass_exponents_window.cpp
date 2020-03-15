#include "mass_exponents_window.h"

MassExponentsWindow::MassExponentsWindow(MFDFA *mfdfa_, QString fileName_, QWidget *parent) : PlotWindow(parent)
{
    mfdfa = mfdfa_;
    fileName = fileName_;
    //set title
    QString winTitle = "Mass exponents - "+fileName;
    setTitle(winTitle);
    //plot
    plotData();
    //plot fields
    addPlotFields();
    //legend
    addLegend();
}

MassExponentsWindow::~MassExponentsWindow(){}

void MassExponentsWindow::plotData()
{
    mfdfa->computeMassExponents();
    mfdfa->plotMassExponents(plt);
    plt->replot();
}

void MassExponentsWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        mfdfa->tauSaveFile(pathToSave.toStdString());
}
