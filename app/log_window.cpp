#include "log_window.h"

LogWindow::LogWindow(QString logs_, QString fileName_, QString fileName2_, QWidget *parent) : QWidget(parent)
{
    logs = logs_;
    fileName = fileName_;
    fileName2 = fileName2_;
    //set dimensions
    setDimensions();
    //set title
    setWindowTitle("Fits log - "+fileName);
    //win size
    setFixedSize(xDim, yDim);
    //save button
    saveBtn = new QPushButton("Save", this);
    saveBtn->setGeometry(xDim-padX/2-xWidth*3, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(onSave()));
    //close button
    closeBtn = new QPushButton("Close", this);
    closeBtn->setGeometry(xDim-padX-xWidth*6, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    //scrollable text area
    txtArea = new QTextEdit(this);
    txtArea->setGeometry(padX*2, padY*4, xDim-padX*2, yDim-yHeight-3*padY);
    txtArea->setText(logs);
    txtArea->setReadOnly(true);
    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(txtArea);
}

LogWindow::~LogWindow(){}

void LogWindow::setDimensions()
{
    xDim = 300;
    yDim = 350;
    xWidth = 30;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void LogWindow::onSave()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if(!pathToSave.isNull() && !pathToSave.isEmpty()){
        QString pathTot;
        QStringList fnList = fileName.split(".");
        QString fn;
        for(int i = 0; i < fnList.size()-1; i++)
            fn.append(fnList[i]);
        pathTot = fn;
        if(fileName2 != ""){
            QStringList fnList2 = fileName2.split(".");
            QString fn2;
            for(int i = 0; i < fnList2.size()-1; i++)
                fn2.append(fnList2[i]);
            pathTot.append("_"+fn2);
        }
        QFile qFile(pathToSave+"/fitsLog_"+pathTot+".txt");
        if (qFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&qFile);
            out << logs;
            qFile.close();
        }
    }
}
