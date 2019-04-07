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
    minWinTxt = new QTextEdit(this);
    minWinTxt->setGeometry(padX+4*xWidth, padY, xWidth, yHeight);
    maxWin = new QLabel("to", this);
    maxWin->setGeometry(2*padX+5*xWidth, padY, xWidth, yHeight);
    maxWinTxt = new QTextEdit(this);
    maxWinTxt->setGeometry(4*padX+5*xWidth, padY, xWidth, yHeight);
    winStep = new QLabel("every", this);
    winStep->setGeometry(5*padX+6*xWidth, padY, xWidth*6/5, yHeight);
    winStepTxt = new QTextEdit("1", this);
    winStepTxt->setGeometry(6*padX+7*xWidth, padY, xWidth, yHeight);
    polOrd = new QLabel("Polynomial order", this);
    polOrd->setGeometry(padX, 2*padY+yHeight, xWidth*4, yHeight);
    polOrdTxt = new QTextEdit("1", this);
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
    pHash->insert("minWin", minWinTxt->toPlainText().trimmed());
    pHash->insert("maxWin", maxWinTxt->toPlainText().trimmed());
    pHash->insert("polOrd", polOrdTxt->toPlainText().trimmed());
    pHash->insert("winStep", winStepTxt->toPlainText().trimmed());
    pHash->insert("revSeg", revSegBox->isChecked() ? "1" : "0");
    close();
//    for(int i = 0;  i < m_dataFiles.size(); i++){
//        DFA dfa = DFA(m_dataFiles[i].toStdString(), mw, Mw, po, sw, rs);
//        qInfo() << dfa.getTsLength();
//        qInfo() << dfa.getRangeLength(mw, Mw);
//        dfa.setFlucVectors();
//        dfa.winFlucComp();
//        dfa.H_loglogFit(mw, Mw);
//        qInfo() << i << dfa.getH_intercept() << dfa.getH();
//    }
}
