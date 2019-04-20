#include "plot_window.h"
#include "FAGlobs.h"
#include "DFA.h"

PlotWindow::PlotWindow(QString fileName, QString analysisType, QHash<QString, QString> *pHash, QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle(analysisType+" - "+fileName.split("/").last());
    //win size
    setFixedSize(xDim, yDim);
    //plot section
    plt = new BasePlot(this);
    plt->setGeometry(xDim/4, padY, xDim*3/4-padX, yDim-yHeight-2*padY);
    plt->SetBasePlot();
    PerformAnalysis(fileName, analysisType, pHash);
    plt->replot();
    //refit button
    refit = new QPushButton("Refit", this);
    refit->setGeometry(xDim-5*xWidth-padX*5/2, yDim-yHeight-padY/2, xWidth, yHeight);
    //replot button
    replot = new QPushButton("Replot", this);
    replot->setGeometry(xDim-4*xWidth-padX*2, yDim-yHeight-padY/2, xWidth, yHeight);
    //save_plot button
    save_plot = new QPushButton("Save plot", this);
    save_plot->setGeometry(xDim-3*xWidth-padX*3/2, yDim-yHeight-padY/2, xWidth, yHeight);
    //save_txt button
    save_txt = new QPushButton("Save data", this);
    save_txt->setGeometry(xDim-2*xWidth-padX, yDim-yHeight-padY/2, xWidth, yHeight);
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-xWidth-padX/2, yDim-yHeight-padY/2, xWidth, yHeight);
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
    //ranges
    xlim = new QLabel("x limits (comma-separated)", this);
    xlim->setGeometry(padX, padY/2, xWidth*7/4, yHeight*2/3);
    xlimTxt = new QTextEdit(QString::number(plt->xAxis->range().lower)+","+QString::number(plt->xAxis->range().upper), this);
    xlimTxt->setGeometry(padX, padY+yHeight*2/3, xWidth*3/2, yHeight);
    ylim = new QLabel("y limits (comma-separated)", this);
    ylim->setGeometry(padX, padY*3/2+yHeight*5/3, xWidth*7/4, yHeight*2/3);
    ylimTxt = new QTextEdit(QString::number(plt->yAxis->range().lower)+","+QString::number(plt->yAxis->range().upper), this);
    ylimTxt->setGeometry(padX, 2*padY+yHeight*7/3, xWidth*3/2, yHeight);
    //labels
    title = new QLabel("Title", this);
    title->setGeometry(padX, padY*5/2+yHeight*10/3, xWidth*7/4, yHeight*2/3);
    titleTxt = new QTextEdit(plt->xAxis2->label(), this);
    titleTxt->setGeometry(padX, 3*padY+yHeight*4, xWidth*3/2, yHeight);
    xlabel = new QLabel("x label", this);
    xlabel->setGeometry(padX, padY*7/2+yHeight*5, xWidth*7/4, yHeight*2/3);
    xlabelTxt = new QTextEdit(plt->xAxis->label(), this);
    xlabelTxt->setGeometry(padX, 4*padY+yHeight*17/3, xWidth*3/2, yHeight);
    ylabel = new QLabel("y label", this);
    ylabel->setGeometry(padX, padY*9/2+yHeight*20/3, xWidth*7/4, yHeight*2/3);
    ylabelTxt = new QTextEdit(plt->yAxis->label(), this);
    ylabelTxt->setGeometry(padX, 5*padY+yHeight*22/3, xWidth*3/2, yHeight);
    //legend
    legend = new QLabel("Legend (semicolon-separated)", this);
    legend->setGeometry(padX, padY*11/2+yHeight*25/3, xWidth*7/4, yHeight*2/3);
    QString lgnd = plt->graph(0)->name();
    for(int i = 1; i < plt->legend->itemCount(); i++)
        lgnd += ";"+plt->graph(i)->name();
    legendTxt = new QTextEdit(this);//lgnd, this);
    legendTxt->setGeometry(padX, 6*padY+yHeight*27/3, xWidth*3/2, 3*yHeight);
}

PlotWindow::~PlotWindow(){}

void PlotWindow::SetDimensions()
{
    xDim = 800;
    yDim = 500;
    xWidth = 110;
    yHeight = 30;
    padX = 10;
    padY = 10;
}

void PlotWindow::PerformAnalysis(QString fileName, QString analysisType, QHash<QString, QString> *pHash)
{
    int mw = pHash->value("minWin").toInt();
    int Mw = pHash->value("maxWin").toInt();
    int po = pHash->value("polOrd").toInt();
    int sw = pHash->value("winStep").toInt();
    int rs = pHash->value("revSeg").toInt();
    DFA dfa = DFA(fileName.toStdString(), mw, Mw, po, sw, rs);
    dfa.setFlucVectors();
    dfa.winFlucComp();
    dfa.H_loglogFit(mw, Mw);
    dfa.plot(plt);
}
