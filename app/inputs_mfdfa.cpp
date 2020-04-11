#include "inputs_mfdfa.h"

InputsMFDFA::InputsMFDFA(QStringList fileNames_, QWidget *parent) : InputsWindow(parent)
{
    fileNames = fileNames_;
    numComp = 5;
    //set title
    setWindowTitle(strMFDFA);
    //set window size
    setFixedSize(xDim, (fileNames.size()*numComp+1)*(yHeight+padY)+2*padY);
    //set graphic components
    setInputsComponents();
}

InputsMFDFA::~InputsMFDFA(){}

void InputsMFDFA::setInputsComponents()
{
    QStringList labels;
    labels.append({"Windows size from", "to", "every"});
    QStringList qLabels;
    qLabels.append({"Total q orders", "from", "every"});
    winsAndStep = new QLineEdit** [fileNames.size()];
    polOrd = new QLineEdit* [fileNames.size()];
    revSeg = new QCheckBox* [fileNames.size()];
    qsAndStep = new QLineEdit** [fileNames.size()];

    for(int i = 0; i < fileNames.size(); i++){
        addLabel(fileNames[i].split("/").last(), i*numComp);

        winsAndStep[i] = new QLineEdit* [labels.size()];
        addLabeledEditBoxArray(labels, winsAndStep[i], i*numComp+1);
        winsAndStep[i][2]->setText("1");

        polOrd[i] = addLabeledEditBox("Polynomial order", i*numComp+2);
        polOrd[i]->setText("1");

        revSeg[i] = addLabeledCheckBox("Backward computation", i*numComp+3);

        qsAndStep[i] = new QLineEdit* [qLabels.size()];
        addLabeledEditBoxArray(qLabels, qsAndStep[i], i*numComp+4);
        qsAndStep[i][2]->setText("1.0");
    }
    addButtons(fileNames.size()*numComp);
}

bool InputsMFDFA::checkInputs()
{
    for(int i = 0; i < fileNames.size(); i++){
        if(!checkFileExistence(fileNames[i].toStdString())){
            QMessageBox messageBox;
            QString errToShow = "File "+fileNames[i].split("/").last()+"not found!";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
            return false;
        }
    }

    mw = new int [fileNames.size()];
    Mw = new int [fileNames.size()];
    ws = new int [fileNames.size()];
    po = new int [fileNames.size()];
    rs = new int [fileNames.size()];
    nq = new int [fileNames.size()];
    qi = new double [fileNames.size()];
    qs = new double [fileNames.size()];
    for(int i = 0; i < fileNames.size(); i++){
        QString mwStr = winsAndStep[i][0]->text().trimmed();
        QString MwStr = winsAndStep[i][1]->text().trimmed();
        QString wsStr = winsAndStep[i][2]->text().trimmed();
        QString poStr = polOrd[i]->text().trimmed();
        QString nqStr = qsAndStep[i][0]->text().trimmed();
        QString qiStr = qsAndStep[i][1]->text().trimmed();
        QString qsStr = qsAndStep[i][2]->text().trimmed();
        if(isCorrectFormat(mwStr) &&
            isCorrectFormat(MwStr) &&
            isCorrectFormat(wsStr) &&
            isCorrectFormat(poStr) &&
            isCorrectFormat(nqStr) &&
            isCorrectFormat(qiStr) &&
            isCorrectFormat(qsStr)){
            mw[i] = mwStr.toInt();
            Mw[i] = MwStr.toInt();
            ws[i] = wsStr.toInt();
            po[i] = poStr.toInt();
            revSeg[i]->isChecked() ? rs[i] = 1 : rs[i] = 0;
            nq[i] = nqStr.toInt();
            qi[i] = qiStr.toDouble();
            qs[i] = qsStr.toDouble();
        }else{
            QMessageBox messageBox;
            QString errToShow = "Inputs must be numeric and not null!";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
            return false;
        }
    }

    QStringList inptErrs;
    for(int i = 0; i < fileNames.size(); i++){
        //windows size
        if(Mw[i] < mw[i])
            inptErrs.append("- biggest scale must be greater than smallest scale\n");
        else if(mw[i] < po[i]+2)
            inptErrs.append("- smallest scale must be greater than "+QString::number(po[i]+1)+"\n");
        //windows step
        if(ws[i] < 1)
            inptErrs.append("- step must be greater than 0\n");
        //polynomial order
        if(po[i] < 1)
            inptErrs.append("- polynomial order must be greater than 0\n");
        //number of q orders
        if(nq[i] < 1)
            inptErrs.append("- number of q orders must be greater than 0\n");
        //qs step
        if(qs[i] <= 0.0)
            inptErrs.append("- step for q orders must be greater than 0\n");
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

void InputsMFDFA::setAnalysisObj()
{
    mfdfa = new MFDFA* [fileNames.size()];
    for(int i = 0; i < fileNames.size(); i++){
        FileOps fo;
        std::string fn = fileNames[i].toStdString();
        int N = fo.rowsNumber(fn);
        FILE *f;
        std::vector<double> vec(N);
        f = fo.openFile(fn, "r");
        for(int j = 0; j < N; j++){
            double tmpVal;
            fscanf(f, "%lf", &tmpVal);
            vec.at(j) = tmpVal;
        }
        fclose(f);
        if(mw[i] > N)
            mw[i] = po[i] + 2;
        if(Mw[i] > N)
            Mw[i] = N;
        mfdfa[i] = new MFDFA(fn, vec, N, mw[i], Mw[i], po[i], qi[i], nq[i], ws[i], qs[i], rs[i]);
    }
}

void InputsMFDFA::onOKClick()
{
    if(checkInputs()){
        setAnalysisObj();
        emit mfdfaInputsInserted(mfdfa);
        close();
    }
}
