#include "plot_window.h"

PlotWindow::PlotWindow(QString analysisType, QHash<QString, QString> *pHash, QString fileName, QString fileName2, QWidget *parent) : QWidget(parent)
{
    //set analysis
    analysis = analysisType;
    //perform analysis
    bool execStop = PerformAnalysis(pHash, fileName, fileName2);
    if(!execStop){
        //set dimensions
        SetDimensions();
        //set title
        QString win_title = analysis+" - "+fileName.split("/").last();
        if(!fileName2.isEmpty())
            win_title.append(" & "+fileName2.split("/").last());
        setWindowTitle(win_title);
        //win size
        setFixedSize(xDim, yDim);
        //plot section
        plt = new BasePlot(setLogAxesByAnalysis(), this);
        plt->setGeometry(xDim/4, padY, xDim*3/4-padX, yDim-yHeight-2*padY);
        plt->SetBasePlot();
        PlotByAnalysis();
        plt->replot();
        //refit button
        if(analysis != strRHODCCA && analysis != strHT && analysis != strMFDFA){
            refit = new QPushButton("Refit", this);
            refit->setGeometry(xDim-5*xWidth-padX*5/2, yDim-yHeight-padY/2, xWidth, yHeight);
            connect(refit, SIGNAL(clicked()), this, SLOT(onRefitClick()));
        }
        //replot button
        replot = new QPushButton("Replot", this);
        replot->setGeometry(xDim-4*xWidth-padX*2, yDim-yHeight-padY/2, xWidth, yHeight);
        connect(replot, SIGNAL(clicked()), this, SLOT(onReplotClick()));
        //save_plot button
        save_plot = new QPushButton("Save plot", this);
        save_plot->setGeometry(xDim-3*xWidth-padX*3/2, yDim-yHeight-padY/2, xWidth, yHeight);
        connect(save_plot, SIGNAL(clicked()), this, SLOT(onSavePlotClick()));
        //save_txt button
        save_txt = new QPushButton("Save data", this);
        save_txt->setGeometry(xDim-2*xWidth-padX, yDim-yHeight-padY/2, xWidth, yHeight);
        connect(save_txt, SIGNAL(clicked()), this, SLOT(onSaveTxtClick()));
        //close button
        close_button = new QPushButton("Close", this);
        close_button->setGeometry(xDim-xWidth-padX/2, yDim-yHeight-padY/2, xWidth, yHeight);
        connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
        //move legend button
        move_legend = new QPushButton("Move legend", this);
        move_legend->setGeometry(padX/2, yDim-yHeight-padY/2, xWidth, yHeight);
        connect(move_legend, SIGNAL(clicked()), this, SLOT(onMoveLegendClick()));
        //ranges
        xlim = new QLabel("x limits (comma-separated)", this);
        xlim->setGeometry(padX, padY/2, xWidth*7/4, yHeight*2/3);
        xlimTxt = new QLineEdit(QString::number(plt->xAxis->range().lower)+","+QString::number(plt->xAxis->range().upper), this);
        xlimTxt->setGeometry(padX, padY+yHeight*2/3, xWidth*3/2, yHeight);
        ylim = new QLabel("y limits (comma-separated)", this);
        ylim->setGeometry(padX, padY*3/2+yHeight*5/3, xWidth*7/4, yHeight*2/3);
        ylimTxt = new QLineEdit(QString::number(plt->yAxis->range().lower)+","+QString::number(plt->yAxis->range().upper), this);
        ylimTxt->setGeometry(padX, 2*padY+yHeight*7/3, xWidth*3/2, yHeight);
        //labels
        title = new QLabel("Title", this);
        title->setGeometry(padX, padY*5/2+yHeight*10/3, xWidth*7/4, yHeight*2/3);
        titleTxt = new QLineEdit(plt->xAxis2->label(), this);
        titleTxt->setGeometry(padX, 3*padY+yHeight*4, xWidth*3/2, yHeight);
        xlabel = new QLabel("x label", this);
        xlabel->setGeometry(padX, padY*7/2+yHeight*5, xWidth*7/4, yHeight*2/3);
        xlabelTxt = new QLineEdit(plt->xAxis->label(), this);
        xlabelTxt->setGeometry(padX, 4*padY+yHeight*17/3, xWidth*3/2, yHeight);
        ylabel = new QLabel("y label", this);
        ylabel->setGeometry(padX, padY*9/2+yHeight*20/3, xWidth*7/4, yHeight*2/3);
        ylabelTxt = new QLineEdit(plt->yAxis->label(), this);
        ylabelTxt->setGeometry(padX, 5*padY+yHeight*22/3, xWidth*3/2, yHeight);
        //legend
        legend = new QLabel("Legend (semicolon-separated)", this);
        legend->setGeometry(padX, padY*11/2+yHeight*25/3, xWidth*7/4, yHeight*2/3);
        QString lgnd = plt->graph(0)->name();
        for(int i = 1; i < plt->legend->itemCount(); i++)
            lgnd += ";"+plt->graph(i)->name();
        legendTxt = new QTextEdit(lgnd.toHtmlEscaped(), this);
        legendTxt->setTabChangesFocus(true);
        legendTxt->setGeometry(padX, 6*padY+yHeight*9, xWidth*3/2, 3*yHeight);
        legendYorN = new QLabel("Hide legend", this);
        legendYorN->setGeometry(padX, 7*padY+yHeight*12, xWidth*4/5, yHeight*2/3);
        legendYorNBox = new QCheckBox(this);
        legendYorNBox->setGeometry(padX+xWidth*4/5, 7*padY+yHeight*12, xWidth, yHeight*2/3);
        connect(legendYorNBox, SIGNAL(clicked()), this, SLOT(onYorNLegend()));
    }else{
        this->deleteLater();
    }
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

bool PlotWindow::setLogAxesByAnalysis()
{
    if(analysis == strDFA || analysis == strDCCA)
        return true;
    else
        return false;
}

bool PlotWindow::PerformAnalysis(QHash<QString, QString> *pHash, QString fileName, QString fileName2)
{
    bool execStop = false;
    if(analysis == strDFA)
        execStop = DFAanalysis(pHash, fileName);
    else if(analysis == strDCCA)
        execStop = DCCAanalysis(pHash, fileName, fileName2);
    else if(analysis == strRHODCCA)
        execStop = rhoDCCAanalysis(pHash, fileName, fileName2);
    else if(analysis == strMFDFA)
        execStop = MFDFAanalysis(pHash, fileName);
    else if(analysis == strHT)
        execStop = HTanalysis(pHash, fileName);
    return execStop;
}

bool PlotWindow::DFAanalysis(QHash<QString, QString> *pHash, QString fileName)
{
    FileOps fo;
    string fn = fileName.toStdString();
    int N = fo.rows_number(fn);
    int mw = pHash->value("minWin").toInt();
    int Mw = pHash->value("maxWin").toInt();
    int po = pHash->value("polOrd").toInt();
    int ws = pHash->value("winStep").toInt();
    int rs = pHash->value("revSeg").toInt();
    if(mw > N)
        mw = 3;
    if(Mw > N)
        Mw = N;
    dfa = new DFA(fn, mw, Mw, po, ws, rs);
    dfa->setFlucVectors();
    bool execStop = dfa->winFlucComp();
    if(!execStop)
        dfa->H_loglogFit(mw, Mw);
    return execStop;
}

bool PlotWindow::DCCAanalysis(QHash<QString, QString> *pHash, QString fileName, QString fileName2)
{
    FileOps fo;
    string fn = fileName.toStdString();
    int N = fo.rows_number(fn);
    string fn2 = fileName2.toStdString();
    int N2 = fo.rows_number(fn2);
    MathOps mo;
    int val = mo.min_val(N, N2);
    int mw = pHash->value("minWin").toInt();
    int Mw = pHash->value("maxWin").toInt();
    int po = pHash->value("polOrd").toInt();
    int ws = pHash->value("winStep").toInt();
    QString ia = pHash->value("isAbs");
    if(mw > N)
        mw = 3;
    if(Mw > val)
        Mw = val;
    dcca = new DCCA(fn, fn2, mw, Mw, po, ia.toStdString(), ws);
    dcca->setFlucVectors();
    bool execStop = dcca->winFlucComp();
    if(!execStop)
        dcca->H_loglogFit(mw, Mw);
    return execStop;
}

bool PlotWindow::rhoDCCAanalysis(QHash<QString, QString> *pHash, QString fileName, QString fileName2)
{
    FileOps fo;
    string fn = fileName.toStdString();
    int N = fo.rows_number(fn);
    string fn2 = fileName2.toStdString();
    int N2 = fo.rows_number(fn2);
    MathOps mo;
    int val = mo.min_val(N, N2);
    int mw = pHash->value("minWin").toInt();
    int Mw = pHash->value("maxWin").toInt();
    int po = pHash->value("polOrd").toInt();
    int ws = pHash->value("winStep").toInt();
    if(mw > N)
        mw = 3;
    if(Mw > val)
        Mw = val;
    rhodcca = new rhoDCCA(fn, fn2, mw, Mw, po, ws);
    bool execStop = rhodcca->computeRho();
    return execStop;
}

bool PlotWindow::MFDFAanalysis(QHash<QString, QString> *pHash, QString fileName)
{
    FileOps fo;
    string fn = fileName.toStdString();
    int N = fo.rows_number(fn);
    int mw = pHash->value("minWin").toInt();
    int Mw = pHash->value("maxWin").toInt();
    int po = pHash->value("polOrd").toInt();
    int ws = pHash->value("winStep").toInt();
    int rs = pHash->value("revSeg").toInt();
    double qi = pHash->value("qIn").toDouble();
    int nq = pHash->value("Nq").toInt();
    double qs = pHash->value("qStep").toDouble();
    if(mw > N)
        mw = 3;
    if(Mw > N)
        Mw = N;
    mfdfa = new MFDFA(fn, mw, Mw, po, qi, nq, ws, qs, rs);
    mfdfa->setFlucVectors();
    bool execStop = mfdfa->winFlucComp();
    return execStop;
}

bool PlotWindow::HTanalysis(QHash<QString, QString> *pHash, QString fileName)
{
    bool execStop = false;
    FileOps fo;
    string fn = fileName.toStdString();
    int N = fo.rows_number(fn);
    if(pHash->value("scale") != ""){
        int sc = pHash->value("scale").toInt();
        int Ns = pHash->value("Nscales").toInt();
        int ss = pHash->value("stepScale").toInt();
        if(sc > N)
            sc = 3;
        if(Ns > N)
            Ns = N - sc + 1;
        if((sc+(Ns-1)*ss) > N)
            Ns = (N - sc) / ss + 1;
        ht = new HT(fn, sc, Ns, ss);
        ht->setFlucVectors();
        execStop = ht->winFlucComp();
    }else{
        string ssc = pHash->value("strScales").toStdString();
        ht = new HT(fn, ssc);
        ht->setFlucVectors();
        execStop = ht->winFlucComp();
    }
    return execStop;
}

void PlotWindow::PlotByAnalysis()
{
    if(analysis == strDFA)
        dfa->plot(plt);
    else if(analysis == strDCCA)
        dcca->plot(plt);
    else if(analysis == strRHODCCA)
        rhodcca->plot(plt);
    else if(analysis == strMFDFA)
        mfdfa->plot(plt);
    else if(analysis == strHT)
        ht->plot(plt);
}

void PlotWindow::DisableButtons()
{
    move_legend->setEnabled(false);
    refit->setEnabled(false);
    replot->setEnabled(false);
    save_plot->setEnabled(false);
    save_txt->setEnabled(false);
    close_button->setEnabled(false);
}

void PlotWindow::EnableButtons()
{
    move_legend->setEnabled(true);
    refit->setEnabled(true);
    replot->setEnabled(true);
    save_plot->setEnabled(true);
    save_txt->setEnabled(true);
    close_button->setEnabled(true);
}

void PlotWindow::onMoveLegendClick()
{
    move_legend_win = new MoveLegendWindow(plt);
    move_legend_win->setAttribute(Qt::WA_DeleteOnClose);
    move_legend_win->setWindowModality(Qt::ApplicationModal);
    move_legend_win->show();
    DisableButtons();
    connect(move_legend_win, SIGNAL(destroyed()), this, SLOT(EnableButtons()));
}

void PlotWindow::onYorNLegend()
{
    if(legendYorNBox->isChecked())
        plt->legend->setVisible(false);
    else
        plt->legend->setVisible(true);
    plt->replot();
}

void PlotWindow::onRefitClick()
{
    refit_win = new RefitWindow();
    refit_win->setAttribute(Qt::WA_DeleteOnClose);
    refit_win->setWindowModality(Qt::ApplicationModal);
    refit_win->show();
    DisableButtons();
    connect(refit_win, SIGNAL(inputsInserted(int, int)), this, SLOT(newFit(int, int)));
    connect(refit_win, SIGNAL(destroyed()), this, SLOT(EnableButtons()));
}

void PlotWindow::newFit(int start, int end)
{
    if(end < start){
        int tmp = end;
        end = start;
        start = tmp;
    }
    double H_intercept = 0.0, H = 0.0;
    RefitByAnalysis(start, end, &H, &H_intercept);
    for(int i = 1; i < plt->graphCount(); i++)
        plt->removeGraph(i);
    int len = end - start + 1;
    QVector<double> n(len), Hfit(len);
    for(int i = 0; i < len; i++){
        n[i] = log(start+i);
        Hfit[i] = H_intercept + H * n[i];
    }
    plt->addGraph();
    plt->graph(plt->graphCount()-1)->setData(n, Hfit);
    QPen pen;
    pen.setWidth(2);
    plt->graph(plt->graphCount()-1)->setPen(pen);
    plt->graph(plt->graphCount()-1)->setName("H = "+QString::number(H));
    plt->graph(plt->graphCount()-1)->rescaleAxes(true);
    plt->replot();
    QString lgnd = plt->graph(0)->name();
    for(int i = 1; i < plt->legend->itemCount(); i++)
        lgnd += ";"+plt->graph(i)->name();
    legendTxt->clear();
    legendTxt->setText(lgnd);
}

void PlotWindow::RefitByAnalysis(int start, int end, double *hSlope, double *hIntcpt)
{
    if(analysis == strDFA){
        dfa->H_loglogFit(start, end);
        *hIntcpt = dfa->getH_intercept();
        *hSlope = dfa->getH();
    }else if(analysis == strDCCA){
        dcca->H_loglogFit(start, end);
        *hIntcpt = dcca->getH_intercept();
        *hSlope = dcca->getH();
    }
}

void PlotWindow::onReplotClick()
{
    QStringList xl = xlimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList yl = ylimTxt->text().replace(QRegExp("\\s+"), "").split(",");
    QStringList lg = legendTxt->toPlainText().split(";");
    QStringList rep_alert;
    QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
    if(xl.size() != 2 ||
       (!xl.first().contains(rgx) || !xl.last().contains(rgx)))
        rep_alert.append("\n- x limits");
    if(yl.size() != 2 ||
       (!yl.first().contains(rgx) || !yl.last().contains(rgx)))
        rep_alert.append("\n- y limits");
    if(lg.size() != plt->legend->itemCount() || (lg.size() == 1 && lg.first() == ""))
        rep_alert.append("\n- legend");
    if(rep_alert.size() == 0){
        plt->xAxis->setRange(xl.first().trimmed().toDouble(), xl.last().trimmed().toDouble());
        plt->yAxis->setRange(yl.first().trimmed().toDouble(), yl.last().trimmed().toDouble());
        plt->xAxis->setLabel(xlabelTxt->text().trimmed());
        plt->yAxis->setLabel(ylabelTxt->text().trimmed());
        plt->xAxis2->setLabel(titleTxt->text().trimmed());
        for(int i = 0; i < plt->legend->itemCount(); i++)
            plt->graph(i)->setName(lg[i].trimmed());
        plt->replot();
    }else{
        QMessageBox messageBox;
        QString errToShow = "An error occured in\n the following fields:\n";
        for(int i = 0; i < rep_alert.size(); i++)
            errToShow.append(rep_alert[i]);
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(200,200);
    }
}

void PlotWindow::onSavePlotClick()
{
    QString save_file;
    QFileDialog save_dialog;
    save_file = save_dialog.getSaveFileName();
    QString file_ext = save_file.split(".").last();
    if(!file_ext.isNull() && !file_ext.isEmpty()){
        if(file_ext == "pdf")
            plt->savePdf(save_file);
        else if(file_ext == "png")
            plt->savePng(save_file);
        else{
            QMessageBox messageBox;
            QString errToShow = "Supported formats are:\n- pdf\n- png";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(200,200);
        }
    }
}

void PlotWindow::onSaveTxtClick()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty()){
        if(analysis == strDFA)
            dfa->saveFile(pathToSave.toStdString());
        else if(analysis == strDCCA)
            dcca->saveFile(pathToSave.toStdString());
        else if(analysis == strRHODCCA)
            rhodcca->saveFile(pathToSave.toStdString());
        else if(analysis == strMFDFA){
            mfdfa->saveFile(pathToSave.toStdString());
            mfdfa->qsaveFile(pathToSave.toStdString());
        }else if(analysis == strHT)
            ht->saveFile(pathToSave.toStdString());
    }
}
