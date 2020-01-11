#include "inputs_ht.h"

InputsHT::InputsHT(QStringList fileNames_, QWidget *parent) : InputsWindow(parent)
{
    fileNames = fileNames_;
    numComp = 4;
    ms = nullptr;
    cs = nullptr;
    //set title
    setWindowTitle(strHT);
    //set window size
    setFixedSize(xDim, (fileNames.size()*numComp+1)*(yHeight+padY)+2*padY);
    //set graphic components
    setInputsComponents();
}

InputsHT::~InputsHT(){}

void InputsHT::setInputsComponents()
{
    QStringList labels;
    labels.append({"Total scales", "from", "every"});
    QStringList mLabels;
    mLabels.append({"MFDFA windows size from", "to", "every"});
    rangeBox = new QCheckBox* [fileNames.size()];
    scalesAndStep = new QLineEdit** [fileNames.size()];
    stringBox = new QCheckBox* [fileNames.size()];
    customScales = new QLineEdit* [fileNames.size()];
    winsAndStep = new QLineEdit** [fileNames.size()];

    for(int i = 0; i < fileNames.size(); i++){
        addLabel(fileNames[i].split("/").last(), i*numComp);

        rangeBox[i] = addCheckBox(i*numComp+1);
        connect(rangeBox[i], &QCheckBox::clicked, [=](){this->onRangeChkBoxClick(i);});

        scalesAndStep[i] = new QLineEdit* [labels.size()];
        addLabeledEditBoxArray(labels, scalesAndStep[i], i*numComp+1, 1);
        scalesAndStep[i][2]->setText("1");
        for(int j = 0; j < labels.size(); j++)
            scalesAndStep[i][j]->setEnabled(false);

        stringBox[i] = addCheckBox(i*numComp+2);
        connect(stringBox[i], &QCheckBox::clicked, [=](){this->onStringChkBoxClick(i);});

        customScales[i] = addLabeledEditBox("Windows sizes (comma separated)", i*numComp+2, 1, 0);
        customScales[i]->setEnabled(false);

        winsAndStep[i] = new QLineEdit* [mLabels.size()];
        addLabeledEditBoxArray(mLabels, winsAndStep[i], i*numComp+3);
        winsAndStep[i][2]->setText("1");
    }
    addButtons(fileNames.size()*numComp);
}

void InputsHT::onRangeChkBoxClick(int idx)
{
    if(rangeBox[idx]->isChecked()){
        scalesAndStep[idx][0]->setEnabled(true);
        scalesAndStep[idx][1]->setEnabled(true);
        scalesAndStep[idx][2]->setEnabled(true);
        stringBox[idx]->setChecked(false);
        customScales[idx]->setText("");
        customScales[idx]->setEnabled(false);
    }else{
        scalesAndStep[idx][0]->setText("");
        scalesAndStep[idx][0]->setEnabled(false);
        scalesAndStep[idx][1]->setText("");
        scalesAndStep[idx][1]->setEnabled(false);
        scalesAndStep[idx][2]->setText("1");
        scalesAndStep[idx][2]->setEnabled(false);
    }
}

void InputsHT::onStringChkBoxClick(int idx)
{
    if(stringBox[idx]->isChecked()){
        customScales[idx]->setEnabled(true);
        rangeBox[idx]->setChecked(false);
        scalesAndStep[idx][0]->setText("");
        scalesAndStep[idx][0]->setEnabled(false);
        scalesAndStep[idx][1]->setText("");
        scalesAndStep[idx][1]->setEnabled(false);
        scalesAndStep[idx][2]->setText("1");
        scalesAndStep[idx][2]->setEnabled(false);
    }else{
        customScales[idx]->setText("");
        customScales[idx]->setEnabled(false);
    }
}

bool InputsHT::checkInputs()
{
    int L = fileNames.size();
    ns = new int [L];
    ms = new int [L];
    ss = new int [L];
    cs = new QString [L];
    for(int i = 0; i < L; i++){
        ns[i] = 0;
        ms[i] = 0;
        ss[i] = 0;
        cs[i] = "";
    }
    mmw = new int [L];
    mMw = new int [L];
    mws = new int [L];
    QStringList inptErrs;
    for(int i = 0; i < L; i++){
        bool err = false;
        if(rangeBox[i]->isChecked()){
            QString nsStr = scalesAndStep[i][0]->text().trimmed();
            QString msStr = scalesAndStep[i][1]->text().trimmed();
            QString ssStr = scalesAndStep[i][2]->text().trimmed();
            if(!msStr.isEmpty()){
                if(isCorrectFormat(nsStr) &&
                    isCorrectFormat(msStr) &&
                    isCorrectFormat(ssStr)){
                    ns[i] = nsStr.toInt();
                    ms[i] = msStr.toInt();
                    ss[i] = ssStr.toInt();
                }else{
                    err = true;
                }
            }
        }else if(stringBox[i]->isChecked()){
            cs[i] = customScales[i]->text().trimmed();
            if(!cs[i].isEmpty()){
                for(QString s : cs[i].split(QString::fromStdString(strSep))){
                    if(!isCorrectFormat(s.trimmed())){
                        err = true;
                        break;
                    }
                }
            }else{
                err = true;
            }
        }
        QString mmwStr = winsAndStep[i][0]->text().trimmed();
        QString mMwStr = winsAndStep[i][1]->text().trimmed();
        QString mwsStr = winsAndStep[i][2]->text().trimmed();
        if(isCorrectFormat(mmwStr) &&
            isCorrectFormat(mMwStr) &&
            isCorrectFormat(mwsStr)){
            mmw[i] = mmwStr.toInt();
            mMw[i] = mMwStr.toInt();
            mws[i] = mwsStr.toInt();
        }else{
            err = true;
        }

        if(err){
            QMessageBox messageBox;
            QString errToShow = "Inputs must be numeric and not null!";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
            return false;
        }

        if(rangeBox[i]->isChecked()){
            //minimum scale size
            if(ms[i] < 3)
                inptErrs.append("- scale must be greater than 2\n");
            //number of scales
            if(ns[i] < 1)
                inptErrs.append("- number of scales must be at least 1\n");
            //step for scales
            if(ss[i] < 1)
                inptErrs.append("- step must be strictly positive and such that scales are smaller than the time series length\n");
        }else if(stringBox[i]->isChecked()){
            for(QString s : cs[i].split(QString::fromStdString(strSep))){
                int sInt = s.toInt();
                if(sInt < 3)
                inptErrs.append("- scale must be greater than 2\n");
            }
        }
        //windows size
        if(mMw[i] < mmw[i])
            inptErrs.append("- MFDFA biggest scale must be greater than smallest scale\n");
        else if(mmw[i] < 3)
            inptErrs.append("- MFDFA smallest scale must be greater than 3\n");
        //windows step
        if(mws[i] < 1)
            inptErrs.append("- MFDFA step must be greater than 0\n");
    }
    
    if(inptErrs.size() > 0){
        QMessageBox messageBox;
        QString errToShow = "The following errors occurred:\n\n";
        inptErrs.removeDuplicates();
        for(int i = 0; i < inptErrs.size(); i++)
            errToShow.append(inptErrs[i]);
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        return false;
    }else{
        return true;
    }
}

void InputsHT::setAnalysisObj()
{
    ht = new HT* [fileNames.size()];
    for(int i = 0; i < fileNames.size(); i++){
        FileOps fo;
        std::string fn = fileNames[i].toStdString();
        int N = fo.rowsNumber(fn);
        if(ms[i] != 0){
            if(ms[i] > N)
                ms[i] = 3;
            if(ns[i] > N)
                ns[i] = N - ms[i] + 1;
            if((ms[i]+(ns[i]-1)*ss[i]) > N)
                ns[i] = (N - ms[i]) / ss[i] + 1;
            ht[i] = new HT(fn, ms[i], ns[i], ss[i], mmw[i], mMw[i], mws[i]);
        }else{
            ht[i] = new HT(fn, cs[i].toStdString(), mmw[i], mMw[i], mws[i]);
        }
    }
}

void InputsHT::onOKClick()
{
    if(checkInputs()){
        setAnalysisObj();
        emit htInputsInserted(ht);
        close();
    }
}
