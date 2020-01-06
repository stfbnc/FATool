#include "refit_window.h"

RefitWindow::RefitWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    setDimensions();
    //set title
    setWindowTitle("Fit parameters");
    //win size
    setFixedSize(xDim, yDim);
    //apply button
    okButton = new QPushButton("OK", this);
    okButton->setGeometry(xDim-padX/2-xWidth*3, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOKClick()));
    //close button
    closeButton = new QPushButton("Close", this);
    closeButton->setGeometry(xDim-padX-xWidth*6, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    //windows sizes
    minWin = new QLabel("Windows size from", this);
    minWin->setGeometry(padX, padY, xWidth*4, yHeight);
    minWinTxt = new QLineEdit(this);
    minWinTxt->setGeometry(padX+4*xWidth, padY, xWidth, yHeight);
    maxWin = new QLabel("to", this);
    maxWin->setGeometry(2*padX+5*xWidth, padY, xWidth, yHeight);
    maxWinTxt = new QLineEdit(this);
    maxWinTxt->setGeometry(4*padX+5*xWidth, padY, xWidth, yHeight);
    //checkboxes
    keepFits = new QCheckBox(this);
    keepFits->setGeometry(padX, padY*2+yHeight, xWidth, yHeight*2/3);
    connect(keepFits, SIGNAL(clicked()), this, SLOT(onKeepFitsClick()));
    keepTxt = new QLabel("Keep previous fit", this);
    keepTxt->setGeometry(padX+xWidth, padY*2+yHeight, xWidth*4, yHeight*2/3);
    clearFits = new QCheckBox(this);
    clearFits->setGeometry(padX, padY*3+yHeight*5/3, xWidth, yHeight*2/3);
    connect(clearFits, SIGNAL(clicked()), this, SLOT(onClearFitsClick()));
    clearTxt = new QLabel("Clear previous fits", this);
    clearTxt->setGeometry(padX+xWidth, padY*3+yHeight*5/3, xWidth*4, yHeight*2/3);
}

RefitWindow::~RefitWindow(){}

void RefitWindow::setDimensions()
{
    xDim = 240;
    yDim = 120;
    xWidth = 30;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void RefitWindow::onOKClick()
{
    QString win1 = minWinTxt->text().trimmed();
    QString win2 = maxWinTxt->text().trimmed();
    QRegExp rgx("^[0-9]+$");
    if((!win1.isEmpty() && win1.contains(rgx)) &&
       (!win2.isEmpty() && win2.contains(rgx))){
        int k, c;
        keepFits->isChecked() ? k = 1 : k = 0;
        clearFits->isChecked() ? c = 1 : c = 0;
        emit inputsInserted(win1.toInt(), win2.toInt(), k, c);
        close();
    }else{
        QMessageBox messageBox;
        QString errToShow = "Inputs must be numeric and not null!";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
}

void RefitWindow::onKeepFitsClick()
{
    clearFits->setChecked(false);
}

void RefitWindow::onClearFitsClick()
{
    keepFits->setChecked(false);
}
