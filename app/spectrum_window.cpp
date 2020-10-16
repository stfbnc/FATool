#include "spectrum_window.h"

SpectrumWindow::SpectrumWindow(MFDFA *mfdfa, QString fileName, QWidget *parent) :
    PlotWindow(parent)
{
    this->mfdfa = mfdfa;

    QString winTitle = "Multifractal spectrum - " + fileName;
    setTitle(winTitle);

    plotData();
    setPlotLimits();
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
