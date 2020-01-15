#include "inputs_dcca.h"

InputsDCCA::InputsDCCA(QStringList fileNames_, QWidget *parent) : InputsWindow(parent)
{
    fileNames = fileNames_;
    numComp = 4;
    MathOps mo;
    combs = mo.binCoeff(fileNames.size(), 2);
    //set title
    setWindowTitle(strDCCA);
    //set window size
    setFixedSize(xDim, (combs*numComp+1)*(yHeight+padY)+2*padY);
    //set graphic components
    setInputsComponents();
}

InputsDCCA::~InputsDCCA(){}

void InputsDCCA::setInputsComponents()
{
    QStringList labels;
    labels.append({"Windows size from", "to", "every"});
    QStringList entries;
    entries.append({"absolute", "signed"});
    fileCouples = new QStringList [combs];
    int couple = 0;
    for(int i = 0; i < fileNames.size()-1; i++){
        for(int j = i+1; j < fileNames.size(); j++){
            fileCouples[couple].append(fileNames[i]);
            fileCouples[couple].append(fileNames[j]);
            couple++;
        }
    }
    winsAndStep = new QLineEdit** [combs];
    polOrd = new QLineEdit* [combs];
    absList = new QComboBox* [combs];

    for(int i = 0; i < combs; i++){
        addLabel(fileCouples[i][0].split("/").last()+" - "+fileCouples[i][1].split("/").last(), i*numComp);

        winsAndStep[i] = new QLineEdit* [labels.size()];
        addLabeledEditBoxArray(labels, winsAndStep[i], i*numComp+1);
        winsAndStep[i][2]->setText("1");

        polOrd[i] = addLabeledEditBox("Polynomial order", i*numComp+2);
        polOrd[i]->setText("1");

        absList[i] = addLabeledComboBox("Computation type", entries, i*numComp+3);
    }
    addButtons(combs*numComp);
}

bool InputsDCCA::checkInputs()
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

    mw = new int [combs];
    Mw = new int [combs];
    ws = new int [combs];
    po = new int [combs];
    al = new std::string [combs];
    for(int i = 0; i < combs; i++){
        QString mwStr = winsAndStep[i][0]->text().trimmed();
        QString MwStr = winsAndStep[i][1]->text().trimmed();
        QString wsStr = winsAndStep[i][2]->text().trimmed();
        QString poStr = polOrd[i]->text().trimmed();
        if(isCorrectFormat(mwStr) &&
            isCorrectFormat(MwStr) &&
            isCorrectFormat(wsStr) &&
            isCorrectFormat(poStr)){
            mw[i] = mwStr.toInt();
            Mw[i] = MwStr.toInt();
            ws[i] = wsStr.toInt();
            po[i] = poStr.toInt();
            absList[i]->currentText()=="absolute" ? al[i] = defaultDCCA : al[i] = corrDCCA;
        }else{
            QMessageBox messageBox;
            QString errToShow = "Inputs must be numeric and not null!";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
            return false;
        }
    }

    QStringList inptErrs;
    for(int i = 0; i < combs; i++){
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

void InputsDCCA::setAnalysisObj()
{
    dcca = new DCCA* [combs];
    for(int i = 0; i < combs; i++){
        FileOps fo;
        std::string fn = fileCouples[i][0].toStdString();
        int N = fo.rowsNumber(fn);
        std::string fn2 = fileCouples[i][1].toStdString();
        int N2 = fo.rowsNumber(fn2);
        FILE *f;
        double *vec, *vec2;
        vec = new double [N];
        vec2 = new double [N2];
        f = fo.openFile(fn, "r");
        for(int j = 0; j < N; j++){
            fscanf(f, "%lf", &vec[j]);
        }
        fclose(f);
        f = fo.openFile(fn2, "r");
        for(int j = 0; j < N2; j++){
            fscanf(f, "%lf", &vec2[j]);
        }
        fclose(f);
        MathOps mo;
        int val = mo.minVal(N, N2);
        if(mw[i] > N)
            mw[i] = po[i] + 2;
        if(Mw[i] > val)
            Mw[i] = val;
        dcca[i] = new DCCA(fn, vec, N, fn2, vec2, N2, mw[i], Mw[i], po[i], al[i], ws[i]);
    }
}

void InputsDCCA::onOKClick()
{
    if(checkInputs()){
        setAnalysisObj();
        emit dccaInputsInserted(dcca);
        close();
    }
}
