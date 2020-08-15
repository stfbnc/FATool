#include "data_plot_window.h"

DataPlotWindow::DataPlotWindow(std::vector<DataFile*> dataFile, std::vector<int> cols, QWidget *parent) :
    PlotWindow(parent)
{
    this->dataFile = dataFile;
    this->cols = cols;

    if(cols.size() > 1)
        setTitle("Single plot");
    else
        setTitle(this->dataFile.at(0)->getFileName());

    plotData();
    setPlotLimits();
    addLegend();
}

DataPlotWindow::~DataPlotWindow(){}

void DataPlotWindow::plotData()
{
    for(int i = 0; i < int(this->cols.size()); i++)
    {
        plt->addGraph();
        std::vector<double> yData = this->dataFile.at(i)->getDataOfColumn(cols.at(i));
        std::vector<double> xData(this->dataFile.at(i)->getDataLength());
        if(this->dataFile.at(i)->getXAxisColumn() != 0)
            xData = this->dataFile.at(i)->getDataOfColumn(this->dataFile.at(i)->getXAxisColumn());
        else
            std::iota(xData.begin(), xData.end(), 0);

        QVector<double> qyData = QVector<double>::fromStdVector(yData);
        QVector<double> qxData = QVector<double>::fromStdVector(xData);

        plt->graph(i)->setData(qxData, qyData);
        plt->graph(i)->setName(this->dataFile.at(i)->getNameOfColumn(cols.at(i)));
        QPen pen;
        pen.setColor(colours.at(i % colours.size()));
        plt->graph(i)->setPen(pen);
        if(i == 0)
            plt->graph(i)->rescaleAxes();
        else
            plt->graph(i)->rescaleAxes(true);
    }
    plt->legend->setVisible(true);
    plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    plt->replot();
}
