#include "inputs_rhodcca.h"

InputsrhoDCCA::InputsrhoDCCA(QStringList fileNames, QStringList columns, QWidget *parent) :
    AbstractInputsWindow(strRHODCCA + " inputs", parent)
{
    this->fileNames = fileNames;
    this->columns = columns;
    this->dataMap = FilesData::getInstance();
    MathOps mo;
    combs = mo.binCoeff(fileNames.size(), 2);

    addWidgets();
}

InputsrhoDCCA::~InputsrhoDCCA(){}

void InputsrhoDCCA::addWidgets()
{
    for(int i = 0; i < fileNames.size()-1; i++)
    {
        for(int j = i+1; j < fileNames.size(); j++)
        {
            fileCouples.append({fileNames.at(i), fileNames.at(j)});
            colsCouples.append({columns.at(i), columns.at(j)});
        }
    }

    for(int i = 0; i < combs; i++)
    {
        addLabel(fileCouples.at(i).at(0).split("/").last() + " - " + fileCouples.at(i).at(1).split("/").last(), true);

        winsAndStep.append(addLabeledLineEdits({"Windows size from", "to", "every"}));
        winsAndStep.at(i).at(2)->setText("1");

        polOrd.append(addLabeledLineEdit({"Polynomial order"}));
        polOrd.at(i)->setText("1");

        compThresh.append(addCheckbox("Compute thresholds"));
    }

    if(combs > 0)
        addThirdButton("Copy to all");
}

bool InputsrhoDCCA::checkInputs()
{
    for(int i = 0; i < combs; i++)
    {
        QString mwStr = winsAndStep.at(i).at(0)->text().trimmed();
        QString MwStr = winsAndStep.at(i).at(1)->text().trimmed();
        QString wsStr = winsAndStep.at(i).at(2)->text().trimmed();
        QString poStr = polOrd.at(i)->text().trimmed();
        if(isCorrectFormat(mwStr) &&
            isCorrectFormat(MwStr) &&
            isCorrectFormat(wsStr) &&
            isCorrectFormat(poStr))
        {
            mw.push_back(mwStr.toInt());
            Mw.push_back(MwStr.toInt());
            ws.push_back(wsStr.toInt());
            po.push_back(poStr.toInt());
            compThresh.at(i)->isChecked() ? ct.push_back(true) : ct.push_back(false);
        }
        else
        {
            QMessageBox messageBox;
            QString errToShow = "Inputs must be numeric and not null!";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
            return false;
        }
    }

    QStringList inptErrs;
    for(int i = 0; i < combs; i++)
    {
        //windows size
        if(Mw.at(i) < mw.at(i))
            inptErrs.append("- biggest scale must be greater than smallest scale\n");
        else if(mw.at(i) < (po.at(i) + 2))
            inptErrs.append("- smallest scale must be greater than "+QString::number(po[i]+1)+"\n");
        //windows step
        if(ws.at(i) < 1)
            inptErrs.append("- step must be greater than 0\n");
        //polynomial order
        if(po.at(i) < 1)
            inptErrs.append("- polynomial order must be greater than 0\n");
    }
    
    if(inptErrs.size() > 0)
    {
        QMessageBox messageBox;
        QString errToShow = "The following errors occurred:\n\n";
        inptErrs.removeDuplicates();
        for(int i = 0; i < inptErrs.size(); i++)
            errToShow.append(inptErrs.at(i));
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        return false;
    }
    else
    {
        return true;
    }
}

void InputsrhoDCCA::onOkClick()
{
    if(checkInputs())
    {
        std::map<QString, DataFile*> map = dataMap->getDataMap();
        for(int i = 0; i < combs; i++)
        {
            std::vector<double> vec = map.at(fileCouples.at(i).at(0))->getDataOfColumn(colsCouples.at(i).at(0).toInt());
            std::vector<double> vec2 = map.at(fileCouples.at(i).at(1))->getDataOfColumn(colsCouples.at(i).at(1).toInt());
            MathOps mo;
            int val = mo.minVal(vec.size(), vec2.size());
            if(mw.at(i) > int(vec.size()))
                mw.at(i) = po.at(i) + 2;
            if(Mw.at(i) > val)
                Mw.at(i) = val;
            FA *fa = new rhoDCCA(fileCouples.at(i).at(0).toStdString(), vec, vec.size(),
                                 fileCouples.at(i).at(1).toStdString(), vec2, vec2.size(),
                                 mw.at(i), Mw.at(i), po.at(i), ws.at(i), ct.at(i));
            rhodcca.push_back(fa);
        }

        emit inputsInserted(rhodcca);
        this->close();
    }
}

void InputsrhoDCCA::onThirdButtonClick()
{
    if(combs > 0)
    {
        QString w1 = winsAndStep.at(0).at(0)->text();
        QString w2 = winsAndStep.at(0).at(1)->text();
        QString w3 = winsAndStep.at(0).at(2)->text();
        QString p = polOrd.at(0)->text();
        bool c = compThresh.at(0)->isChecked();

        for(int i = 1; i < combs; i++)
        {
            winsAndStep.at(i).at(0)->setText(w1);
            winsAndStep.at(i).at(1)->setText(w2);
            winsAndStep.at(i).at(2)->setText(w3);
            polOrd.at(i)->setText(p);
            compThresh.at(i)->setChecked(c);
        }
    }
}
