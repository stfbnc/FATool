#include "mfdfa_window.h"

MFDFAWindow::MFDFAWindow(MFDFA *mfdfa_, QWidget *parent) : PlotWindow(parent)
{
    mfdfa = mfdfa_;
    fileName = QString::fromStdString(mfdfa->getFileName()).split("/").last();
    //set title
    QString winTitle = strMFDFA+" - "+fileName;
    setTitle(winTitle);
    //spectrum button
    addSpectrumButton();
    //mass exponents button
    addMassExponentsButton();
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

void MFDFAWindow::onSpectrumClick()
{
    spectWin = new SpectrumWindow(mfdfa, fileName);
    spectWin->setAttribute(Qt::WA_DeleteOnClose);
    spectWin->show();
}

void MFDFAWindow::onMassExponentsClick()
{
    massExpWin = new MassExponentsWindow(mfdfa, fileName);
    massExpWin->setAttribute(Qt::WA_DeleteOnClose);
    massExpWin->show();
}

void MFDFAWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        mfdfa->saveFile(pathToSave.toStdString());
}
