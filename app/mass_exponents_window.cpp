#include "mass_exponents_window.h"

MassExponentsWindow::MassExponentsWindow(MFDFA *mfdfa, QString fileName, QWidget *parent) :
    PlotWindow(parent)
{
    this->mfdfa = mfdfa;

    QString winTitle = "Mass exponents - " + fileName;
    setTitle(winTitle);

    plotData();
    setPlotLimits();
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
