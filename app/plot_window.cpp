#include "plot_window.h"

PlotWindow::PlotWindow(QWidget *parent) : QWidget(parent)
{
    fileName2 = "";
	//win size
	setFixedSize(xDim, yDim);
	//plot section
    plt = new BasePlot(this);
    plt->setGeometry(xDim/4, padY, xDim*3/4-padX, yDim-yHeight-2*padY);
    plt->setBasePlot();
    //not plot section
    refitBtn = nullptr;
    fitLogBtn = nullptr;
    spectBtn = nullptr;
    massExpBtn = nullptr;
    addButtons();
}

PlotWindow::~PlotWindow(){}

void PlotWindow::setTitle(QString winTitle)
{
    setWindowTitle(winTitle);
}

void PlotWindow::addButtons()
{
	//replot button
	replotBtn = new QPushButton("Replot", this);
	replotBtn->setGeometry(xDim-4*xWidth-padX*2, yDim-yHeight-padY/2, xWidth, yHeight);
	connect(replotBtn, SIGNAL(clicked()), this, SLOT(onReplotClick()));
	//save_plot button
	savePlotBtn = new QPushButton("Save plot", this);
	savePlotBtn->setGeometry(xDim-3*xWidth-padX*3/2, yDim-yHeight-padY/2, xWidth, yHeight);
	connect(savePlotBtn, SIGNAL(clicked()), this, SLOT(onSavePlotClick()));
	//save_txt button
	saveTxtBtn = new QPushButton("Save data", this);
	saveTxtBtn->setGeometry(xDim-2*xWidth-padX, yDim-yHeight-padY/2, xWidth, yHeight);
	connect(saveTxtBtn, SIGNAL(clicked()), this, SLOT(onSaveTxtClick()));
	//close button
	closeBtn = new QPushButton("Close", this);
	closeBtn->setGeometry(xDim-xWidth-padX/2, yDim-yHeight-padY/2, xWidth, yHeight);
	connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
	//move legend button
	moveLegendBtn = new QPushButton("Move legend", this);
	moveLegendBtn->setGeometry(padX/2, yDim-yHeight-padY/2, xWidth, yHeight);
	connect(moveLegendBtn, SIGNAL(clicked()), this, SLOT(onMoveLegendClick()));
}

void PlotWindow::addRefitButton()
{
	//refit button
	refitBtn = new QPushButton("Refit", this);
	refitBtn->setGeometry(xDim-5*xWidth-padX*5/2, yDim-yHeight-padY/2, xWidth, yHeight);
	connect(refitBtn, SIGNAL(clicked()), this, SLOT(onRefitClick()));
}

void PlotWindow::addFitLogButton()
{
    //fit log button
    fitLogBtn = new QPushButton("Fits log", this);
    fitLogBtn->setGeometry(xDim-6*xWidth-padX*3, yDim-yHeight-padY/2, xWidth, yHeight);
    connect(fitLogBtn, SIGNAL(clicked()), this, SLOT(onFitLogClick()));
}

void PlotWindow::addSpectrumButton()
{
    //spectrum button
    spectBtn = new QPushButton("Spectrum", this);
    spectBtn->setGeometry(xDim-5*xWidth-padX*5/2, yDim-yHeight-padY/2, xWidth, yHeight);
    connect(spectBtn, SIGNAL(clicked()), this, SLOT(onSpectrumClick()));
}

void PlotWindow::addMassExponentsButton()
{
    //mass exponents button
    massExpBtn = new QPushButton("Mass exps", this);
    massExpBtn->setGeometry(xDim-6*xWidth-padX*3, yDim-yHeight-padY/2, xWidth, yHeight);
    connect(massExpBtn, SIGNAL(clicked()), this, SLOT(onMassExponentsClick()));
}

void PlotWindow::addPlotFields()
{
	//ranges
	xLimLbl = new QLabel("x limits (comma-separated)", this);
	xLimLbl->setGeometry(padX, padY/2, xWidth*7/4, yHeight*2/3);
    xLimTxt = new QLineEdit(QString::number(plt->xAxis->range().lower)+","+QString::number(plt->xAxis->range().upper), this);
    xLimTxt->setGeometry(padX, padY+yHeight*2/3, xWidth*3/2, yHeight);
	yLimLbl = new QLabel("y limits (comma-separated)", this);
	yLimLbl->setGeometry(padX, padY*3/2+yHeight*5/3, xWidth*7/4, yHeight*2/3);
    yLimTxt = new QLineEdit(QString::number(plt->yAxis->range().lower)+","+QString::number(plt->yAxis->range().upper), this);
    yLimTxt->setGeometry(padX, 2*padY+yHeight*7/3, xWidth*3/2, yHeight);
	//labels
	titleLbl = new QLabel("Title", this);
	titleLbl->setGeometry(padX, padY*5/2+yHeight*10/3, xWidth*7/4, yHeight*2/3);
	titleTxt = new QLineEdit(plt->xAxis2->label(), this);
	titleTxt->setGeometry(padX, 3*padY+yHeight*4, xWidth*3/2, yHeight);
	xLabelLbl = new QLabel("x label", this);
	xLabelLbl->setGeometry(padX, padY*7/2+yHeight*5, xWidth*7/4, yHeight*2/3);
	xLabelTxt = new QLineEdit(plt->xAxis->label(), this);
	xLabelTxt->setGeometry(padX, 4*padY+yHeight*17/3, xWidth*3/2, yHeight);
	yLabelLbl = new QLabel("y label", this);
	yLabelLbl->setGeometry(padX, padY*9/2+yHeight*20/3, xWidth*7/4, yHeight*2/3);
	yLabelTxt = new QLineEdit(plt->yAxis->label(), this);
	yLabelTxt->setGeometry(padX, 5*padY+yHeight*22/3, xWidth*3/2, yHeight);
}

void PlotWindow::addLegend()
{
	//legend
	legendLbl = new QLabel("Legend (semicolon-separated)", this);
	legendLbl->setGeometry(padX, padY*11/2+yHeight*25/3, xWidth*7/4, yHeight*2/3);
	QString lgnd = plt->graph(0)->name();
	for(int i = 1; i < plt->legend->itemCount(); i++)
		lgnd += ";"+plt->graph(i)->name();
	legendTxt = new QTextEdit(lgnd.toHtmlEscaped(), this);
	legendTxt->setTabChangesFocus(true);
	legendTxt->setGeometry(padX, 6*padY+yHeight*9, xWidth*3/2, 3*yHeight);
	isLegendLbl = new QLabel("Hide legend", this);
	isLegendLbl->setGeometry(padX, 7*padY+yHeight*12, xWidth*4/5, yHeight*2/3);
	isLegendBox = new QCheckBox(this);
	isLegendBox->setGeometry(padX+xWidth*4/5, 7*padY+yHeight*12, xWidth, yHeight*2/3);
	connect(isLegendBox, SIGNAL(clicked()), this, SLOT(onIsLegendCheck()));
}

void PlotWindow::plotData(){}

void PlotWindow::disableButtons()
{
    moveLegendBtn->setEnabled(false);
    replotBtn->setEnabled(false);
    savePlotBtn->setEnabled(false);
    saveTxtBtn->setEnabled(false);
    closeBtn->setEnabled(false);
	if(refitBtn != nullptr)
		refitBtn->setEnabled(false);
    if(fitLogBtn != nullptr)
        fitLogBtn->setEnabled(false);
    if(spectBtn != nullptr)
        spectBtn->setEnabled(false);
    if(massExpBtn != nullptr)
        massExpBtn->setEnabled(false);
}

void PlotWindow::enableButtons()
{
    moveLegendBtn->setEnabled(true);
    replotBtn->setEnabled(true);
    savePlotBtn->setEnabled(true);
    saveTxtBtn->setEnabled(true);
    closeBtn->setEnabled(true);
	if(refitBtn != nullptr)
		refitBtn->setEnabled(true);
    if(fitLogBtn != nullptr)
        fitLogBtn->setEnabled(true);
    if(spectBtn != nullptr)
        spectBtn->setEnabled(true);
    if(massExpBtn != nullptr)
        massExpBtn->setEnabled(true);
}

void PlotWindow::onSpectrumClick(){}

void PlotWindow::onMassExponentsClick(){}

void PlotWindow::onFitLogClick()
{
    logWin = new LogWindow(fitLog, fileName, fileName2);
    logWin->setAttribute(Qt::WA_DeleteOnClose);
    logWin->show();
    disableButtons();
    connect(logWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
}

void PlotWindow::onMoveLegendClick()
{
    moveLegendWin = new MoveLegendWindow(plt);
    moveLegendWin->setAttribute(Qt::WA_DeleteOnClose);
    moveLegendWin->setWindowModality(Qt::ApplicationModal);
    moveLegendWin->show();
    disableButtons();
    connect(moveLegendWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
}

void PlotWindow::onIsLegendCheck()
{
    if(isLegendBox->isChecked())
        plt->legend->setVisible(false);
    else
        plt->legend->setVisible(true);
    plt->replot();
}

void PlotWindow::onRefitClick()
{
    refitWin = new RefitWindow();
    refitWin->setAttribute(Qt::WA_DeleteOnClose);
    refitWin->setWindowModality(Qt::ApplicationModal);
    refitWin->show();
    disableButtons();
    connect(refitWin, SIGNAL(inputsInserted(int, int, int, int)), this, SLOT(newFitPlot(int, int, int, int)));
    connect(refitWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
}

void PlotWindow::newFitPlot(int start, int end, int keep, int clear)
{
    if(end < start){
        int tmp = end;
        end = start;
        start = tmp;
    }
    double HIntercept = 0.0, H = 0.0;
    refitData(start, end, &H, &HIntercept);
    if(clear == 1){
        for(int i = 1; i < plt->graphCount(); i++)
            plt->removeGraph(i);
    }
    if(keep == 0 && plt->graphCount() != 1){
        plt->removeGraph(plt->graphCount()-1);
    }
    int len = end - start + 1;
    QVector<double> n(len), Hfit(len);
    for(int i = 0; i < len; i++){
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
    legendTxt->clear();
    legendTxt->setText(lgnd);

    fitLog.append("start: "+QString::number(start)+
                   "\nend: "+QString::number(end)+
                   "\nH: "+QString::number(H)+
                   "\nintercept: "+QString::number(HIntercept)+
                   "\n----------------------------\n");
}

void PlotWindow::refitData(int start, int end, double *hSlope, double *hIntcpt){}

void PlotWindow::onReplotClick()
{
    QStringList xl = xLimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList yl = yLimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList lg = legendTxt->toPlainText().split(";");
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
    if(repAlert.size() == 0){
        plt->xAxis->setRange(xl.first().trimmed().toDouble(), xl.last().trimmed().toDouble());
        plt->yAxis->setRange(yl.first().trimmed().toDouble(), yl.last().trimmed().toDouble());
        plt->xAxis->setLabel(xLabelTxt->text().trimmed());
        plt->yAxis->setLabel(yLabelTxt->text().trimmed());
        plt->xAxis2->setLabel(titleTxt->text().trimmed());
        for(int i = 0; i < plt->legend->itemCount(); i++)
            plt->graph(i)->setName(lg[i].trimmed());
        plt->replot();
    }else{
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
    if(!fileExt.isNull() && !fileExt.isEmpty()){
        if(fileExt == "pdf")
            plt->savePdf(saveFile);
        else if(fileExt == "png")
            plt->savePng(saveFile);
        else{
            QMessageBox messageBox;
            QString errToShow = "Supported formats are:\n- pdf\n- png";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
    }
}

void PlotWindow::onSaveTxtClick(){}
