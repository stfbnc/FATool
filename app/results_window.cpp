#include "results_window.h"

ResultsWindow::ResultsWindow(FA *fa, QWidget *parent) :
    PlotWindow(parent)
{
    this->fa = fa;
    QString f1 = QString::fromStdString(fa->getFileName()).split("/").last();
    QString f2 = QString::fromStdString(fa->getFileName2()).split("/").last();
    QString winTitle = fa->getAlgorithmStr() + " - " + f1;
    if(f2 != "")
        winTitle.append(" & " + f2);
    setTitle(winTitle);

    if(fa->isFittable())
        addFitButtons();
    if(fa->getAlgorithmStr() == strMFDFA)
    {
        addSpectrumButton();
        addMassExponentsButton();
    }
    plotData();
    setPlotLimits();
    addLegend();
}

ResultsWindow::~ResultsWindow(){}

void ResultsWindow::plotData()
{
    fa->plot(plt);
    plt->setLogAxes(fa->getLogType());
    plt->replot();
}

void ResultsWindow::refitData(int start, int end, double *hSlope, double *hIntcpt)
{
    if(fa->isFittable())
    {
        fa->executeFit(start, end);
        *hIntcpt = fa->getHintercept();
        *hSlope = fa->getH();
    }
}

void ResultsWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        fa->saveFile(pathToSave.toStdString());
}

void ResultsWindow::onMassExponentsClick()
{
    MFDFA *obj = (MFDFA *)fa;
    MassExponentsWindow *massExpWin = new MassExponentsWindow(obj, QString::fromStdString(obj->getFileName()));
    massExpWin->show();
}

void ResultsWindow::onSpectrumClick()
{
    MFDFA *obj = (MFDFA *)fa;
    SpectrumWindow *spectWin = new SpectrumWindow(obj, QString::fromStdString(obj->getFileName()));
    spectWin->show();
}
