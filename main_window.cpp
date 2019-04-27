#include <QDebug>
#include "main_window.h"
#include "FAGlobs.h"
#include "FileOps.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //set parameters hash
    initializeParamHash();
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle("FATool");
    //win size
    setFixedSize(xDim, yDim);
    //plot section
    qplot = new BasePlot(this);
    qplot->setGeometry(padX, padY+yHeight, xDim-2*padX, yDim-yHeight-2*padY);
    qplot->SetBasePlot();
    qplot->xAxis->setLabel("time");
    qplot->yAxis->setLabel("Time series");
    qplot->replot();
    //load button
    load_button = new QPushButton("Load file(s)", this);
    load_button->setGeometry(padX/2, padY/2, xWidth, yHeight);
    connect(load_button, SIGNAL(clicked()), this, SLOT(onLoadClick()));
    //save button
    save_button = new QPushButton("Save plot", this);
    save_button->setGeometry(padX/2+xWidth, padY/2, xWidth, yHeight);
    connect(save_button, SIGNAL(clicked()), this, SLOT(onSaveClick()));
    //text
    fluct_txt = new QLabel("Type of analysis:", this);
    fluct_txt->setGeometry(padX+2*xWidth, padY/2, xWidth, yHeight);
    fluct_txt->setStyleSheet("font-weight: bold");
    //dropdown list
    dd_list = new QComboBox(this);
    FillList();
    dd_list->setGeometry(padX*3/2+3*xWidth, padY/2+2, xWidth, yHeight);
    //go button
    go_button = new QPushButton("Go!", this);
    go_button->setGeometry(padX*2+4*xWidth, padY/2, xWidth, yHeight);
    connect(go_button, SIGNAL(clicked()), this, SLOT(onGoClick()));
    //clear button
    clear_button = new QPushButton("Clear all", this);
    clear_button->setGeometry(padX*5/2+5*xWidth, padY/2, xWidth, yHeight);
    connect(clear_button, SIGNAL(clicked()), this, SLOT(onClearClick()));
    //quit button
    quit_button = new QPushButton("Quit", this);
    quit_button->setGeometry(xDim-xWidth-padX/2, padY/2, xWidth, yHeight);
    connect(quit_button, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}

MainWindow::~MainWindow(){}

void MainWindow::SetDimensions()
{
    xDim = 800;
    yDim = 500;
    xWidth = 110;
    yHeight = 30;
    padX = 10;
    padY = 10;
}

void MainWindow::initializeParamHash()
{
    paramHash = new QHash<QString, QString>;
    paramHash->insert("minWin", "");
    paramHash->insert("maxWin", "");
    paramHash->insert("polOrd", "");
    paramHash->insert("winStep", "");
    paramHash->insert("revSeg", "");
    paramHash->insert("qIn", "");
    paramHash->insert("Nq", "");
    paramHash->insert("qStep", "");
    paramHash->insert("isAbs", "");
    paramHash->insert("scale", "");
    paramHash->insert("Nscales", "");
    paramHash->insert("stepScale", "");
    paramHash->insert("strScales", "");
}

void MainWindow::FillList()
{
    dd_list->addItem("-");
    dd_list->addItem(strDFA);
    dd_list->addItem(strMFDFA);
    dd_list->addItem(strDCCA);
    dd_list->addItem(strHT);
    dd_list->addItem(strRHODCCA);
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
        string fn = fileNames.at(i).toLocal8Bit().constData();
        FileOps fo;
        int len = fo.rows_number(fn);
        QVector<double> t(len), vec(len);
        FILE *f;
        f = fo.open_file(fn, "r");
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
        DisableButtons();
        save_win = new SaveWindow(qplot);
        save_win->setAttribute(Qt::WA_DeleteOnClose);
        save_win->show();
        connect(save_win, SIGNAL(destroyed()), this, SLOT(EnableButtons()));
    }
}

void MainWindow::DisableButtons()
{
    load_button->setEnabled(false);
    go_button->setEnabled(false);
    save_button->setEnabled(false);
    dd_list->setEnabled(false);
    clear_button->setEnabled(false);
}

void MainWindow::EnableButtons()
{
    load_button->setEnabled(true);
    go_button->setEnabled(true);
    save_button->setEnabled(true);
    dd_list->setEnabled(true);
    clear_button->setEnabled(true);
}

void MainWindow::onGoClick()
{
    QString analysisType = dd_list->currentText();
    if(analysisType != "-" && qplot->graphCount() > 0){
        if((analysisType == strDCCA || analysisType == strRHODCCA) && fileNames.size() < 2){
            QMessageBox messageBox;
            QString errToShow = "This type of analysis requires\nat least two data files";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(200,200);
        }else{
            DisableButtons();
            inpt_win = new InputsWindow(analysisType, paramHash);
            inpt_win->setAttribute(Qt::WA_DeleteOnClose);
            inpt_win->show();
            connect(inpt_win, SIGNAL(destroyed()), this, SLOT(EnableButtons()));
            connect(inpt_win, SIGNAL(inputsInserted()), this, SLOT(onCloseInputWin()), Qt::QueuedConnection);
        }
    }
}

void MainWindow::onCloseInputWin()
{
    QString analysisType = dd_list->currentText();
    if(analysisType == strDFA){
        for(int i = 0;  i < fileNames.size(); i++){
            PlotWindow *plot_win = new PlotWindow(analysisType, paramHash, fileNames[i]);
            plot_win->setAttribute(Qt::WA_DeleteOnClose);
            plot_win->show();
        }
    }
    if(analysisType == strDCCA || analysisType == strRHODCCA){
        for(int i = 0;  i < fileNames.size()-1; i++){
            for(int j = i+1; j < fileNames.size(); j++){
                PlotWindow *plot_win = new PlotWindow(analysisType, paramHash, fileNames[i], fileNames[j]);
                plot_win->setAttribute(Qt::WA_DeleteOnClose);
                plot_win->show();
            }
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
