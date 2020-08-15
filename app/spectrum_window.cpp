#include "spectrum_window.h"

SpectrumWindow::SpectrumWindow(MFDFA *mfdfa_, QString fileName_, QWidget *parent) : PlotWindow(parent)
{
    mfdfa = mfdfa_;
    fileName = fileName_;
    //set title
    QString winTitle = "Multifractal spectrum - "+fileName;
    setTitle(winTitle);
    //plot
    plotData();
    //plot fields
    setPlotLimits();
    //legend
    addLegend();
}

SpectrumWindow::~SpectrumWindow(){}

void SpectrumWindow::plotData()
{
    mfdfa->computeSpectrum();
    mfdfa->plotSpectrum(plt);
    plt->replot();
}

void SpectrumWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        mfdfa->spectrumSaveFile(pathToSave.toStdString());
}
