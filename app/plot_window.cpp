#include "plot_window.h"
#include "ui_plot_window.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->fileName2 = "";

    plt = new BasePlot(this);
    plt->setBasePlot();
    ui->plotLayout->addWidget(plt);

    ui->refitBtn->hide();
    ui->fitLogBtn->hide();
    ui->spectBtn->hide();
    ui->massExpBtn->hide();

    connect(ui->replotBtn, SIGNAL(clicked()), this, SLOT(onReplotClick()));
    connect(ui->savePlotBtn, SIGNAL(clicked()), this, SLOT(onSavePlotClick()));
    connect(ui->saveTxtBtn, SIGNAL(clicked()), this, SLOT(onSaveTxtClick()));
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->moveLegendBtn, SIGNAL(clicked()), this, SLOT(onMoveLegendClick()));
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::setTitle(QString winTitle)
{
    setWindowTitle(winTitle);
}

void PlotWindow::addFitButtons()
{
    ui->refitBtn->show();
    connect(ui->refitBtn, SIGNAL(clicked()), this, SLOT(onRefitClick()));

    ui->fitLogBtn->show();
    connect(ui->fitLogBtn, SIGNAL(clicked()), this, SLOT(onFitLogClick()));
}

void PlotWindow::addSpectrumButton()
{
    ui->spectBtn->show();
    connect(ui->spectBtn, SIGNAL(clicked()), this, SLOT(onSpectrumClick()));
}

void PlotWindow::addMassExponentsButton()
{
    ui->massExpBtn->show();
    connect(ui->massExpBtn, SIGNAL(clicked()), this, SLOT(onMassExponentsClick()));
}

void PlotWindow::setPlotLimits()
{
    ui->xLimTxt->setText(QString::number(plt->xAxis->range().lower)+","+QString::number(plt->xAxis->range().upper));
    ui->yLimTxt->setText(QString::number(plt->yAxis->range().lower)+","+QString::number(plt->yAxis->range().upper));
}

void PlotWindow::addLegend()
{
    QString lgnd = plt->graph(0)->name();
	for(int i = 1; i < plt->legend->itemCount(); i++)
        lgnd += ";"+plt->graph(i)->name();

    ui->legendTxt->setText(lgnd.toHtmlEscaped());
    ui->legendTxt->setTabChangesFocus(true);

    connect(ui->isLegendBox, SIGNAL(clicked()), this, SLOT(onIsLegendCheck()));
}

void PlotWindow::plotData(){}

void PlotWindow::disableButtons()
{
    ui->moveLegendBtn->setEnabled(false);
    ui->replotBtn->setEnabled(false);
    ui->savePlotBtn->setEnabled(false);
    ui->saveTxtBtn->setEnabled(false);
    ui->closeBtn->setEnabled(false);
    if(!ui->refitBtn->isHidden())
        ui->refitBtn->setEnabled(false);
    if(!ui->fitLogBtn->isHidden())
        ui->fitLogBtn->setEnabled(false);
    if(!ui->spectBtn->isHidden())
        ui->spectBtn->setEnabled(false);
    if(!ui->massExpBtn->isHidden())
        ui->massExpBtn->setEnabled(false);
}

void PlotWindow::enableButtons()
{
    ui->moveLegendBtn->setEnabled(true);
    ui->replotBtn->setEnabled(true);
    ui->savePlotBtn->setEnabled(true);
    ui->saveTxtBtn->setEnabled(true);
    ui->closeBtn->setEnabled(true);
    if(!ui->refitBtn->isHidden())
        ui->refitBtn->setEnabled(true);
    if(!ui->fitLogBtn->isHidden())
        ui->fitLogBtn->setEnabled(true);
    if(!ui->spectBtn->isHidden())
        ui->spectBtn->setEnabled(true);
    if(!ui->massExpBtn->isHidden())
        ui->massExpBtn->setEnabled(true);
}

void PlotWindow::onSpectrumClick(){}

void PlotWindow::onMassExponentsClick(){}

void PlotWindow::onFitLogClick()
{
    LogWindow *logWin = new LogWindow(fitLog, fileName, fileName2);
    logWin->show();
}

void PlotWindow::onMoveLegendClick()
{
    MoveLegendWindow *moveLegendWin = new MoveLegendWindow(plt);
    moveLegendWin->show();
}

void PlotWindow::onIsLegendCheck()
{
    if(ui->isLegendBox->isChecked())
        plt->legend->setVisible(false);
    else
        plt->legend->setVisible(true);
    plt->replot();
}

void PlotWindow::onRefitClick()
{
    RefitWindow *refitWin = new RefitWindow();
    refitWin->show();
    connect(refitWin, SIGNAL(inputsInserted(int, int, int, int)), this, SLOT(newFitPlot(int, int, int, int)));
}

void PlotWindow::newFitPlot(int start, int end, int keep, int clear)
{
    if(end < start)
    {
        int tmp = end;
        end = start;
        start = tmp;
    }

    double HIntercept = 0.0, H = 0.0;
    refitData(start, end, &H, &HIntercept);

    if(clear == 1)
    {
        for(int i = 1; i < plt->graphCount(); i++)
            plt->removeGraph(i);
    }

    if(keep == 0 && plt->graphCount() != 1)
    {
        plt->removeGraph(plt->graphCount()-1);
    }

    int len = end - start + 1;
    QVector<double> n(len), Hfit(len);
    for(int i = 0; i < len; i++)
    {
        n[i] = log(start+i);
        Hfit[i] = HIntercept + H * n[i];
    }

    plt->addGraph();
    plt->graph(plt->graphCount()-1)->setData(n, Hfit);
    QPen pen;
    pen.setWidth(2);
    pen.setStyle(lineStyles[(plt->graphCount()-1)%lineStyles.size()]);
    plt->graph(plt->graphCount()-1)->setPen(pen);
    plt->graph(plt->graphCount()-1)->setName("H = "+QString::number(H));
    plt->graph(plt->graphCount()-1)->rescaleAxes(true);
    plt->replot();
    QString lgnd = plt->graph(0)->name();
    for(int i = 1; i < plt->legend->itemCount(); i++)
        lgnd += ";"+plt->graph(i)->name();
    ui->legendTxt->clear();
    ui->legendTxt->setText(lgnd);

    fitLog.append("start: "+QString::number(start)+
                   "\nend: "+QString::number(end)+
                   "\nH: "+QString::number(H)+
                   "\nintercept: "+QString::number(HIntercept)+
                   "\n----------------------------\n");
}

void PlotWindow::refitData(int start, int end, double *hSlope, double *hIntcpt)
{
    start = 0;
    end = 0;
    *hSlope = 0.0;
    *hIntcpt = 0.0;
}

void PlotWindow::onReplotClick()
{
    QStringList xl = ui->xLimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList yl = ui->yLimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList lg = ui->legendTxt->toPlainText().split(";");
    QStringList repAlert;
    QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");

    if(xl.size() != 2 ||
       (!xl.first().contains(rgx) || !xl.last().contains(rgx)))
        repAlert.append("\n- x limits");
    if(yl.size() != 2 ||
       (!yl.first().contains(rgx) || !yl.last().contains(rgx)))
        repAlert.append("\n- y limits");
    if(lg.size() != plt->legend->itemCount() || (lg.size() == 1 && lg.first() == ""))
        repAlert.append("\n- legend");

    if(repAlert.size() == 0)
    {
        plt->xAxis->setRange(xl.first().trimmed().toDouble(), xl.last().trimmed().toDouble());
        plt->yAxis->setRange(yl.first().trimmed().toDouble(), yl.last().trimmed().toDouble());
        plt->xAxis->setLabel(ui->xLabelTxt->text().trimmed());
        plt->yAxis->setLabel(ui->yLabelTxt->text().trimmed());
        plt->xAxis2->setLabel(ui->titleTxt->text().trimmed());
        for(int i = 0; i < plt->legend->itemCount(); i++)
            plt->graph(i)->setName(lg[i].trimmed());
        plt->replot();
    }
    else
    {
        QMessageBox messageBox;
        QString errToShow = "An error occured in\n the following fields:\n";
        for(int i = 0; i < repAlert.size(); i++)
            errToShow.append(repAlert[i]);
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
}

void PlotWindow::onSavePlotClick()
{
    QString saveFile;
    QFileDialog saveDialog;
    saveFile = saveDialog.getSaveFileName();
    QString fileExt = saveFile.split(".").last();

    if(!fileExt.isNull() && !fileExt.isEmpty())
    {
        if(fileExt == "pdf")
        {
            plt->savePdf(saveFile);
        }
        else if(fileExt == "png")
        {
            plt->savePng(saveFile);
        }
        else
        {
            QMessageBox messageBox;
            QString errToShow = "Supported formats are:\n- pdf\n- png";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
    }
}

void PlotWindow::onSaveTxtClick(){}
