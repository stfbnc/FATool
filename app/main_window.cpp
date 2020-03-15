#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    setDimensions();
    //set title
    setWindowTitle("FATool (v "+VERSION+")");
    //win size
    setFixedSize(xDim, yDim);
    //plot section
    qplot = new BasePlot(this);
    qplot->setGeometry(padX, padY+yHeight, xDim-2*padX, yDim-yHeight-2*padY);
    qplot->setBasePlot();
    qplot->xAxis->setLabel("time");
    qplot->yAxis->setLabel("Time series");
    qplot->replot();
    //load button
    loadButton = new QPushButton("Load file(s)", this);
    loadButton->setGeometry(padX/2, padY/2, xWidth, yHeight);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(onLoadClick()));
    //save button
    saveButton = new QPushButton("Save plot", this);
    saveButton->setGeometry(padX/2+xWidth, padY/2, xWidth, yHeight);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(onSaveClick()));
    //dropdown text
    analysisLbl = new QLabel("Type of analysis:", this);
    analysisLbl->setGeometry(padX+2*xWidth, padY/2, xWidth, yHeight);
    analysisLbl->setStyleSheet("font-weight: bold");
    //dropdown list
    ddList = new QComboBox(this);
    fillList();
    ddList->setGeometry(padX*3/2+3*xWidth, padY/2+2, xWidth, yHeight);
    //go button
    goButton = new QPushButton("Go!", this);
    goButton->setGeometry(padX*2+4*xWidth, padY/2, xWidth, yHeight);
    connect(goButton, SIGNAL(clicked()), this, SLOT(onGoClick()));
    //clear button
    clearButton = new QPushButton("Clear all", this);
    clearButton->setGeometry(padX*5/2+5*xWidth, padY/2, xWidth, yHeight);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(onClearClick()));
    //quit button
    quitButton = new QPushButton("Quit", this);
    quitButton->setGeometry(xDim-xWidth-padX/2, padY/2, xWidth, yHeight);
    connect(quitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    //instructions window
    instrWindow();
}

MainWindow::~MainWindow(){}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event->Close)
        QApplication::quit();
}

void MainWindow::setDimensions()
{
    xDim = 800;
    yDim = 500;
    xWidth = 110;
    yHeight = 30;
    padX = 10;
    padY = 10;
}

void MainWindow::instrWindow()
{
    QFile f(prefsFile);
    QTextStream in(&f);
    QString str;
    if(f.open(QFile::ReadWrite)){
        str = in.readLine();
        f.close();
    }
    if(str == showStartWin)
        startWin = new StartingWindow();
}

void MainWindow::fillList()
{
    ddList->addItem("-");
    ddList->addItem(strDFA);
    ddList->addItem(strMFDFA);
    ddList->addItem(strDCCA);
    ddList->addItem(strHT);
    ddList->addItem(strRHODCCA);
}

void MainWindow::onLoadClick()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(dataFilter);
    if(dialog.exec())
        fileNames.append(dialog.selectedFiles());
    fileNames.removeDuplicates();
    qplot->legend->clearItems();
    qplot->clearGraphs();
    for(int i = 0; i < fileNames.size(); i++){
        std::string fn = fileNames.at(i).toLocal8Bit().constData();
        FileOps fo;
        int len = fo.rowsNumber(fn);
        QVector<double> t(len), vec(len);
        FILE *f;
        f = fo.openFile(fn, "r");
        if(f){
            for(int j = 0; j < len; j++){
                t[j] = j;
                fscanf(f, "%lf", &vec[j]);
            }
        }
        fclose(f);
        qplot->addGraph();
        QPen pen;
        pen.setColor(colours[i%colours.size()]);
        qplot->graph(i)->setPen(pen);
        qplot->graph(i)->setName(QString::fromStdString(fn).split("/").last());
        qplot->graph(i)->setData(t, vec);
        i==0 ? qplot->graph(i)->rescaleAxes() : qplot->graph(i)->rescaleAxes(true);
    }
    if(fileNames.size() > 0){
        qplot->legend->setVisible(true);
        qplot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
        qplot->replot();
    }
}

void MainWindow::onSaveClick()
{
    if(qplot->graphCount() > 0){
        disableButtons();
        saveWin = new SaveWindow(qplot);
        saveWin->setAttribute(Qt::WA_DeleteOnClose);
        saveWin->setWindowModality(Qt::ApplicationModal);
        saveWin->show();
        connect(saveWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
    }
}

void MainWindow::disableButtons()
{
    loadButton->setEnabled(false);
    goButton->setEnabled(false);
    saveButton->setEnabled(false);
    ddList->setEnabled(false);
    clearButton->setEnabled(false);
}

void MainWindow::enableButtons()
{
    loadButton->setEnabled(true);
    goButton->setEnabled(true);
    saveButton->setEnabled(true);
    ddList->setEnabled(true);
    clearButton->setEnabled(true);
}

void MainWindow::onGoClick()
{
    QString analysisType = ddList->currentText();
    if(analysisType != "-" && qplot->graphCount() > 0){
        if((analysisType == strDCCA || analysisType == strRHODCCA) && fileNames.size() < 2){
            QMessageBox messageBox;
            QString errToShow = "This type of analysis requires\nat least two data files";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }else{
            disableButtons();
            if(analysisType == strDFA){
                dfaInptWin = new InputsDFA(fileNames);
                dfaInptWin->setAttribute(Qt::WA_DeleteOnClose);
                dfaInptWin->setWindowModality(Qt::ApplicationModal);
                dfaInptWin->show();
                connect(dfaInptWin, SIGNAL(dfaInputsInserted(DFA**)), this, SLOT(onCloseDFAInputWin(DFA**)), Qt::QueuedConnection);
                connect(dfaInptWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
            }else if(analysisType == strDCCA){
                dccaInptWin = new InputsDCCA(fileNames);
                dccaInptWin->setAttribute(Qt::WA_DeleteOnClose);
                dccaInptWin->setWindowModality(Qt::ApplicationModal);
                dccaInptWin->show();
                connect(dccaInptWin, SIGNAL(dccaInputsInserted(DCCA**)), this, SLOT(onCloseDCCAInputWin(DCCA**)), Qt::QueuedConnection);
                connect(dccaInptWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
            }else if(analysisType == strMFDFA){
                mfdfaInptWin = new InputsMFDFA(fileNames);
                mfdfaInptWin->setAttribute(Qt::WA_DeleteOnClose);
                mfdfaInptWin->setWindowModality(Qt::ApplicationModal);
                mfdfaInptWin->show();
                connect(mfdfaInptWin, SIGNAL(mfdfaInputsInserted(MFDFA**)), this, SLOT(onCloseMFDFAInputWin(MFDFA**)), Qt::QueuedConnection);
                connect(mfdfaInptWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
            }else if(analysisType == strRHODCCA){
                rhodccaInptWin = new InputsrhoDCCA(fileNames);
                rhodccaInptWin->setAttribute(Qt::WA_DeleteOnClose);
                rhodccaInptWin->setWindowModality(Qt::ApplicationModal);
                rhodccaInptWin->show();
                connect(rhodccaInptWin, SIGNAL(rhodccaInputsInserted(rhoDCCA**)), this, SLOT(onCloseRHODCCAInputWin(rhoDCCA**)), Qt::QueuedConnection);
                connect(rhodccaInptWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
            }else if(analysisType == strHT){
                htInptWin = new InputsHT(fileNames);
                htInptWin->setAttribute(Qt::WA_DeleteOnClose);
                htInptWin->setWindowModality(Qt::ApplicationModal);
                htInptWin->show();
                connect(htInptWin, SIGNAL(htInputsInserted(HT**)), this, SLOT(onCloseHTInputWin(HT**)), Qt::QueuedConnection);
                connect(htInptWin, SIGNAL(destroyed()), this, SLOT(enableButtons()));
            }
        }
    }
}

void MainWindow::onCloseDFAInputWin(DFA **dfa)
{
    for(int i = 0;  i < fileNames.size(); i++){
        dfa[i]->setFlucVectors();
        bool execStop = dfa[i]->computeFlucVec();
        if(!execStop){
            dfa[i]->fitFlucVec(dfa[i]->getMinWin(), dfa[i]->getMaxWin());
            DFAWindow *plotWin = new DFAWindow(dfa[i]);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}

void MainWindow::onCloseDCCAInputWin(DCCA **dcca)
{
    MathOps mo;
    long long int combs = mo.binCoeff(fileNames.size(), 2);
    for(int i = 0;  i < combs; i++){
        dcca[i]->setFlucVectors();
        bool execStop = dcca[i]->computeFlucVec();
        if(!execStop){
            dcca[i]->fitFlucVec(dcca[i]->getMinWin(), dcca[i]->getMaxWin());
            DCCAWindow *plotWin = new DCCAWindow(dcca[i]);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}

void MainWindow::onCloseMFDFAInputWin(MFDFA **mfdfa)
{
    for(int i = 0;  i < fileNames.size(); i++){
        mfdfa[i]->setFlucVectors();
        bool execStop = mfdfa[i]->computeFlucVec();
        if(!execStop){
            MFDFAWindow *plotWin = new MFDFAWindow(mfdfa[i]);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}

void MainWindow::onCloseRHODCCAInputWin(rhoDCCA **rhodcca)
{
    MathOps mo;
    long long int combs = mo.binCoeff(fileNames.size(), 2);
    for(int i = 0;  i < combs; i++){
        bool execStop = rhodcca[i]->computeRho();
        if(!execStop){
            if(rhodcca[i]->threshCompute())
                rhodcca[i]->computeThresholds();
            rhoDCCAWindow *plotWin = new rhoDCCAWindow(rhodcca[i]);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}

void MainWindow::onCloseHTInputWin(HT **ht)
{
    for(int i = 0;  i < fileNames.size(); i++){
        ht[i]->setFlucVectors();
        bool execStop = ht[i]->computeFlucVec();
        if(!execStop){
            HTWindow *plotWin = new HTWindow(ht[i]);
            plotWin->setAttribute(Qt::WA_DeleteOnClose);
            plotWin->show();
        }
    }
}

void MainWindow::onClearClick()
{
    if(qplot->graphCount() > 0){
        fileNames.clear();
        qplot->legend->setVisible(false);
        qplot->legend->clearItems();
        qplot->clearGraphs();
        qplot->replot();
    }
}
