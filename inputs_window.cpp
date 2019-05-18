#include "inputs_window.h"

InputsWindow::InputsWindow(QString analysisStr, QHash<QString, QString> *pHash, QWidget *parent) : QWidget(parent)
{
    //set dimensions
    SetDimensions();
    //set analysis
    analysis = analysisStr;
    //set title
    setWindowTitle(analysis);
    //win size
    setFixedSize(xDim, yDim);
    //ok button
    ok_button = new QPushButton("OK", this);
    ok_button->setGeometry(xDim-padX/2-xWidth*3, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(ok_button, &QPushButton::clicked, [=](){this->onOKClick(pHash);});
    //close button
    close_button = new QPushButton("Close", this);
    close_button->setGeometry(xDim-padX-xWidth*6, yDim-yHeight-padY, xWidth*3, yHeight);
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
    setInptWindow();
}

InputsWindow::~InputsWindow(){}

void InputsWindow::SetDimensions()
{
    xDim = 310;
    yDim = 180;
    xWidth = 30;
    yHeight = 30;
    padX = 10;
    padY = 5;
}

void InputsWindow::setInptWindow()
{
    if(analysis == strDFA)
        DFAInptWindow();
    else if(analysis == strDCCA)
        DCCAInptWindow();
    else if(analysis == strMFDFA)
        MFDFAInptWindow();
    else if(analysis == strRHODCCA)
        rhoDCCAInptWindow();
    else if(analysis == strHT)
        HTInptWindow();
}

void InputsWindow::DFAInptWindow()
{
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

void InputsWindow::DCCAInptWindow()
{
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
    isAbs = new QLabel("Computation type:", this);
    isAbs->setGeometry(padX, 3*padY+2*yHeight, xWidth*5, yHeight);
    absList = new QComboBox(this);
    absList->setGeometry(padX/2+4*xWidth, 3*padY+2*yHeight+1, xWidth*10/3, yHeight);
    absList->addItem("absolute");
    absList->addItem("signed");
}

void InputsWindow::MFDFAInptWindow()
{
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
    Nq = new QLabel("Total q orders", this);
    Nq->setGeometry(padX, 4*padY+3*yHeight, xWidth*4, yHeight);
    NqTxt = new QLineEdit(this);
    NqTxt->setGeometry(padX+3*xWidth, 4*padY+3*yHeight, xWidth, yHeight);
    qIn = new QLabel("   from", this);
    qIn->setGeometry(2*padX+xWidth*4, 4*padY+3*yHeight, xWidth*2, yHeight);
    qInTxt = new QLineEdit(this);
    qInTxt->setGeometry(4*padX+5*xWidth, 4*padY+3*yHeight, xWidth, yHeight);
    qStep = new QLabel("every", this);
    qStep->setGeometry(5*padX+6*xWidth, 4*padY+3*yHeight, xWidth*6/5, yHeight);
    qStepTxt = new QLineEdit("1.0", this);
    qStepTxt->setGeometry(6*padX+7*xWidth, 4*padY+3*yHeight, xWidth, yHeight);
}

void InputsWindow::rhoDCCAInptWindow()
{
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
}

void InputsWindow::HTInptWindow()
{
    rangeBox = new QCheckBox(this);
    rangeBox->setGeometry(padX, padY, xWidth, yHeight);
    connect(rangeBox, SIGNAL(clicked()), this, SLOT(onRangeChkBoxClick()));
    Nq = new QLabel("Total scales", this);
    Nq->setGeometry(padX+xWidth, padY, xWidth*4, yHeight);
    NqTxt = new QLineEdit(this);
    NqTxt->setGeometry(padX+xWidth*11/3, padY, xWidth, yHeight);
    NqTxt->setEnabled(false);
    qIn = new QLabel("    from", this);
    qIn->setGeometry(2*padX+xWidth*4+2, padY, xWidth*2, yHeight);
    qInTxt = new QLineEdit(this);
    qInTxt->setGeometry(4*padX+5*xWidth+2, padY, xWidth, yHeight);
    qInTxt->setEnabled(false);
    qStep = new QLabel("every", this);
    qStep->setGeometry(5*padX+6*xWidth, padY, xWidth*6/5, yHeight);
    qStepTxt = new QLineEdit("1", this);
    qStepTxt->setGeometry(6*padX+7*xWidth, padY, xWidth, yHeight);
    qStepTxt->setEnabled(false);
    stringBox = new QCheckBox(this);
    stringBox->setGeometry(padX, 2*padY+yHeight, xWidth, yHeight);
    connect(stringBox, SIGNAL(clicked()), this, SLOT(onStringChkBoxClick()));
    polOrd = new QLabel("Windows sizes (comma separated)", this);
    polOrd->setGeometry(padX+xWidth, 2*padY+yHeight, xWidth*22/3, yHeight);
    polOrdTxt = new QLineEdit(this);
    polOrdTxt->setGeometry(padX+xWidth, 3*padY+2*yHeight, xWidth*22/3, yHeight);
    polOrdTxt->setEnabled(false);
}

void InputsWindow::onRangeChkBoxClick()
{
    if(rangeBox->isChecked()){
        NqTxt->setEnabled(true);
        qInTxt->setEnabled(true);
        qStepTxt->setEnabled(true);
        stringBox->setChecked(false);
        polOrdTxt->setText("");
        polOrdTxt->setEnabled(false);
    }else{
        NqTxt->setText("");
        NqTxt->setEnabled(false);
        qInTxt->setText("");
        qInTxt->setEnabled(false);
        qStepTxt->setText("1");
        qStepTxt->setEnabled(false);
    }
}

void InputsWindow::onStringChkBoxClick()
{
    if(stringBox->isChecked()){
        polOrdTxt->setEnabled(true);
        rangeBox->setChecked(false);
        NqTxt->setText("");
        NqTxt->setEnabled(false);
        qInTxt->setText("");
        qInTxt->setEnabled(false);
        qStepTxt->setText("1");
        qStepTxt->setEnabled(false);
    }else{
        polOrdTxt->setText("");
        polOrdTxt->setEnabled(false);
    }
}

bool InputsWindow::CheckInputs(QHash<QString, QString> *pHash)
{
    bool chk = false;
    bool chkHTstr = false;
    QStringList inpt_errs;
    int mw = pHash->value("minWin").toInt();
    int Mw = pHash->value("maxWin").toInt();
    int po = pHash->value("polOrd").toInt();
    int ws = pHash->value("winStep").toInt();
    int Nq = pHash->value("Nq").toInt();
    double qs = pHash->value("qStep").toDouble();
    int sc = 0, Ns = 0, ss = 0;
    QString sts;
    if(!pHash->value("scale").isEmpty()){
        sc = pHash->value("scale").toInt();
        Ns = pHash->value("Nscales").toInt();
        ss = pHash->value("stepScale").toInt();
    }else{
        sts = pHash->value("strScales");
        chkHTstr = true;
    }
    //DFA, DCCA, rhoDCCA
    if(analysis == strDFA || analysis == strDCCA || analysis == strRHODCCA
       || analysis == strMFDFA){
        //windows size
        if(Mw < mw)
            inpt_errs.append("- biggest scale must be greater than smallest scale\n");
        else if(mw < 3)
            inpt_errs.append("- smallest scale must be greater than 2\n");
        //windows step
        if(ws < 1)
            inpt_errs.append("- step must be greater than 0\n");
        //polynomial order
        if(po < 1)
            inpt_errs.append("- polynomial order must be greater than 0\n");
    }
    //MFDFA
    if(analysis == strMFDFA){
        //number of q orders
        if(Nq < 1)
            inpt_errs.append("- number of q orders must be greater than 0\n");
        if(qs <= 0.0)
            inpt_errs.append("- step for q orders must be greater than 0\n");
    }
    //HT
    if(analysis == strHT){
        if(!chkHTstr){
            //windows size
            if(sc < 3)
                inpt_errs.append("- scale must be greater than 2\n");
            if(Ns < 1)
                inpt_errs.append("- number of scales must be at least 1\n");
            if(ss < 1)
                inpt_errs.append("- step must be strictly positive and such that scales are smaller than the time series length\n");
        }else{
            for(QString s : sts.split(QString::fromStdString(STR_SEP))){
                int sInt = s.toInt();
                if(sInt < 3)
                    inpt_errs.append("- scale must be greater than 2\n");
            }
        }
    }
    if(inpt_errs.size() > 0){
        QMessageBox messageBox;
        QString errToShow = "The following errors occurred:\n\n";
        inpt_errs.removeDuplicates();
        for(int i = 0; i < inpt_errs.size(); i++)
            errToShow.append(inpt_errs[i]);
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(200,200);
    }else{
        chk = true;
    }
    return chk;
}

void InputsWindow::onOKClick(QHash<QString, QString> *pHash)
{
    bool inptOk;
    QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
    if(analysis != strHT){
        QString mw = minWinTxt->text().trimmed();
        QString Mw = maxWinTxt->text().trimmed();
        QString po = polOrdTxt->text().trimmed();
        QString ws = winStepTxt->text().trimmed();
        if((!mw.isEmpty() && mw.contains(rgx)) &&
           (!Mw.isEmpty() && Mw.contains(rgx)) &&
           (!po.isEmpty() && po.contains(rgx)) &&
           (!ws.isEmpty() && ws.contains(rgx))){
            pHash->insert("minWin", mw);
            pHash->insert("maxWin", Mw);
            pHash->insert("polOrd", po);
            pHash->insert("winStep", ws);
            if(analysis == strDFA || analysis == strMFDFA)
                pHash->insert("revSeg", revSegBox->isChecked() ? "1" : "0");
            if(analysis == strDCCA)
                pHash->insert("isAbs", absList->currentText()=="absolute" ? DEFAULT_DCCA : CORR_DCCA);
            inptOk = true;
        }else{
            inptOk = false;
        }
        if(analysis == strMFDFA){
            QString qi = qInTxt->text().trimmed();
            QString nq = NqTxt->text().trimmed();
            QString qs = qStepTxt->text().trimmed();
            if((!qi.isEmpty() && qi.contains(rgx)) &&
               (!nq.isEmpty() && nq.contains(rgx)) &&
               (!qs.isEmpty() && qs.contains(rgx))){
                pHash->insert("qIn", qi);
                pHash->insert("Nq", nq);
                pHash->insert("qStep", qs);
                inptOk = true;
            }else{
                inptOk = false;
            }
        }
    }else{
        QString sc = qInTxt->text().trimmed();
        QString Ns = NqTxt->text().trimmed();
        QString ss = qStepTxt->text().trimmed();
        QRegExp spaces("\\s+");
        QString sts = polOrdTxt->text().trimmed().replace(spaces, "");
        if((!sc.isEmpty() && sc.contains(rgx)) &&
           (!Ns.isEmpty() && Ns.contains(rgx)) &&
           (!ss.isEmpty() && ss.contains(rgx))){
            pHash->insert("scale", sc);
            pHash->insert("Nscales", Ns);
            pHash->insert("stepScale", ss);
            pHash->insert("strScales", "");
            inptOk = true;
        }else{
            if(!sts.isEmpty()){
                bool noNum = false;
                for(QString s : sts.split(QString::fromStdString(STR_SEP))){
                    if(!s.contains(rgx)){
                        noNum = true;
                        break;
                    }
                }
                if(!noNum){
                    pHash->insert("scale", "");
                    pHash->insert("Nscales", "");
                    pHash->insert("stepScale", "");
                    pHash->insert("strScales", sts);
                    inptOk = true;
                }else{
                    inptOk = false;
                }
            }else{
                inptOk = false;
            }
        }
    }
    if(inptOk){
        if(CheckInputs(pHash)){
            emit inputsInserted();
            close();
        }
    }else{
        QMessageBox messageBox;
        QString errToShow = "Inputs must be numeric and not null!";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(200,200);
    }
}
