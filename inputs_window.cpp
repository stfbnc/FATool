#include "inputs_window.h"

InputsWindow::InputsWindow(QString analysisStr, QHash<QString, QString> *pHash, QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set title
    setWindowTitle(analysisStr);
    //win size
    setFixedSize(xDim, yDim);
    //apply button
    ok_button = new QPushButton("OK", this);
    ok_button->setGeometry(xDim-padX/2-xWidth*3, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(ok_button, &QPushButton::clicked, [=](){this->onOKClick(pHash);});
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
    winStep = new QLabel("every", this);
    winStep->setGeometry(5*padX+6*xWidth, padY, xWidth*6/5, yHeight);
    winStepTxt = new QLineEdit("1", this);
    winStepTxt->setGeometry(6*padX+7*xWidth, padY, xWidth, yHeight);
    polOrd = new QLabel("Polynomial order", this);
    polOrd->setGeometry(padX, 2*padY+yHeight, xWidth*4, yHeight);
    polOrdTxt = new QLineEdit("1", this);
    polOrdTxt->setGeometry(padX+4*xWidth, 2*padY+yHeight, xWidth, yHeight);
    revSeg = new QLabel("Backward computation", this);
    revSeg->setGeometry(padX, 3*padY+2*yHeight, xWidth*5, yHeight);
    revSegBox = new QCheckBox(this);
    revSegBox->setGeometry(padX+5*xWidth, 3*padY+2*yHeight, 2*xWidth, yHeight);
}

InputsWindow::~InputsWindow(){}

void InputsWindow::SetDimensions()
{
    xDim = 310;
    yDim = 145;
    xWidth = 30;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void InputsWindow::onOKClick(QHash<QString, QString> *pHash)
{
    QString mw = minWinTxt->text().trimmed();
    QString Mw = maxWinTxt->text().trimmed();
    QString po = polOrdTxt->text().trimmed();
    QString ws = winStepTxt->text().trimmed();
    QRegExp rgx("^[0-9]+$");
    if((!mw.isEmpty() && mw.contains(rgx)) &&
       (!Mw.isEmpty() && Mw.contains(rgx)) &&
       (!po.isEmpty() && po.contains(rgx)) &&
       (!ws.isEmpty() && ws.contains(rgx))){
        pHash->insert("minWin", mw);
        pHash->insert("maxWin", Mw);
        pHash->insert("polOrd", po);
        pHash->insert("winStep", ws);
        pHash->insert("revSeg", revSegBox->isChecked() ? "1" : "0");
        emit inputsInserted();
        close();
    }else{
        QMessageBox messageBox;
        QString errToShow = "Inputs must be numeric and not null!";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(200,200);
    }
}
