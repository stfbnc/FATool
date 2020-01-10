#include "inputs_window.h"

InputsWindow::InputsWindow(QWidget *parent) : QWidget(parent)
{
    //set dimensions
    setDimensions();
}

InputsWindow::~InputsWindow(){}

void InputsWindow::setDimensions()
{
    xDim = 400;
    xWidth = 30;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void InputsWindow::addLabel(QString lbl, int row)
{
    QLabel *label = new QLabel(lbl, this);
    int w = label->fontMetrics().boundingRect(label->text()).width()+padX;
    label->setStyleSheet("font-weight: bold");
    label->setGeometry(padX, (row+1)*padY+row*yHeight, w, yHeight);
}

void InputsWindow::addButtons(int row)
{
    //ok button
    okButton = new QPushButton("OK", this);
    okButton->setGeometry(xDim-padX/2-xWidth*3, (row+1)*padY+row*yHeight, xWidth*3, yHeight);
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOKClick()), Qt::QueuedConnection);
    //close button
    closeButton = new QPushButton("Close", this);
    closeButton->setGeometry(xDim-padX-xWidth*6, (row+1)*padY+row*yHeight, xWidth*3, yHeight);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

QLineEdit* InputsWindow::addLabeledEditBox(QString txt, int row, int col)
{
    int pos;
    if(col == 0)
        pos = padX;
    else
        pos = 4 * padX;
    QLabel *label = new QLabel(txt, this);
    int w = label->fontMetrics().boundingRect(label->text()).width()+padX;
    label->setGeometry(pos, (row+1)*padY+row*yHeight, w, yHeight);
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(pos+w, (row+1)*padY+row*yHeight, xWidth*4, yHeight);
    return lineEdit;
}

void InputsWindow::addLabeledEditBoxArray(QStringList txt, QLineEdit **lineEdits, int row, int col)
{
    int lastPos;
    if(col == 0)
        lastPos = padX;
    else
        lastPos = 4 * padX;
    for(int i = 0; i < txt.size(); i++){
        QLabel *label = new QLabel(txt[i], this);
        int w = label->fontMetrics().boundingRect(label->text()).width()+padX;
        label->setGeometry(lastPos, (row+1)*padY+row*yHeight, w, yHeight);
        lastPos += w;
        lineEdits[i] = new QLineEdit(this);
        lineEdits[i]->setGeometry(lastPos, (row+1)*padY+row*yHeight, xWidth, yHeight);
        lastPos += xWidth + padX;
    }
}

QCheckBox* InputsWindow::addCheckBox(int row)
{
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setGeometry(padX, (row+1)*padY+row*yHeight, xWidth, yHeight);
    return checkBox;
}

QCheckBox* InputsWindow::addLabeledCheckBox(QString txt, int row)
{
    QLabel *label = new QLabel(txt, this);
    int w = label->fontMetrics().boundingRect(label->text()).width()+padX;
    label->setGeometry(padX, (row+1)*padY+row*yHeight, w, yHeight);
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setGeometry(padX+w, (row+1)*padY+row*yHeight, xWidth, yHeight);
    return checkBox;
}

QComboBox* InputsWindow::addLabeledComboBox(QString txt, QStringList elements, int row)
{
    QLabel *label = new QLabel(txt, this);
    int w = label->fontMetrics().boundingRect(label->text()).width()+padX;
    label->setGeometry(padX, (row+1)*padY+row*yHeight, w, yHeight);
    QComboBox *comboBox = new QComboBox(this);
    comboBox->setGeometry(padX+w, (row+1)*padY+row*yHeight, xWidth*10/3, yHeight);
    for(int i = 0; i < elements.size(); i++)
        comboBox->addItem(elements[i]);
    return comboBox;
}

bool InputsWindow::isCorrectFormat(QString txt)
{
    QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
    if(!txt.isEmpty() && txt.contains(rgx))
        return true;
    else
        return false;
}

bool InputsWindow::checkInputs()
{
    return false;
}

void InputsWindow::setInputsComponents(){}

void InputsWindow::setAnalysisObj(){}

void InputsWindow::onOKClick(){}
