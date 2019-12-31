#include "ht_window.h"

HTWindow::HTWindow(HT *ht_, QWidget *parent) : PlotWindow(parent)
{
    ht = ht_;
    //set title
    QString winTitle = strHT+" - "+QString::fromStdString(ht->getFileName()).split("/").last();
    setTitle(winTitle);
    //plot
    plotData();
    //plot fields
    addPlotFields();
    //legend
    addLegend();
}

HTWindow::~HTWindow(){}

void HTWindow::plotData()
{
    ht->plot(plt);
    plt->replot();
}

void HTWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty())
        ht->saveFile(pathToSave.toStdString());
}
