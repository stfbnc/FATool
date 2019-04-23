#include "refit_window.h"

RefitWindow::RefitWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle("Fit parameters");
    //win size
    setFixedSize(xDim, yDim);
    //apply button
    ok_button = new QPushButton("OK", this);
    ok_button->setGeometry(xDim-padX/2-xWidth*3, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(ok_button, SIGNAL(clicked()), this, SLOT(onOKClick()));
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-padX-xWidth*6, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
    //windows sizes
    minWin = new QLabel("Windows size from", this);
    minWin->setGeometry(padX, padY, xWidth*4, yHeight);
    minWinTxt = new QLineEdit(this);
    minWinTxt->setGeometry(padX+4*xWidth, padY, xWidth, yHeight);
    maxWin = new QLabel("to", this);
    maxWin->setGeometry(2*padX+5*xWidth, padY, xWidth, yHeight);
    maxWinTxt = new QLineEdit(this);
    maxWinTxt->setGeometry(4*padX+5*xWidth, padY, xWidth, yHeight);
}

RefitWindow::~RefitWindow(){}

void RefitWindow::SetDimensions()
{
    xDim = 240;
    yDim = 80;
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
        emit inputsInserted(win1.toInt(), win2.toInt());
        close();
    }else{
        QMessageBox messageBox;
        QString errToShow = "Inputs must be numeric and not null!";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(200,200);
    }
}
