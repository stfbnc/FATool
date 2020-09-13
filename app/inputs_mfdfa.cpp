#include "inputs_mfdfa.h"

InputsMFDFA::InputsMFDFA(QStringList fileNames, QStringList columns, FilesData *dataMap, QWidget *parent) :
    AbstractInputsWindow(strMFDFA + " inputs", parent)
{
    this->fileNames = fileNames;
    this->columns = columns;
    this->dataMap = dataMap;

    addWidgets();
}

InputsMFDFA::~InputsMFDFA(){}

void InputsMFDFA::addWidgets()
{
    for(int i = 0; i < fileNames.size(); i++)
    {
        addLabel(fileNames.at(i).split("/").last(), true);

        winsAndStep.append(addLabeledLineEdits({"Windows size from", "to", "every"}));
        winsAndStep.at(i).at(2)->setText("1");

        polOrd.append(addLabeledLineEdit({"Polynomial order"}));
        polOrd.at(i)->setText("1");

        revSeg.append(addCheckbox("Backward computation"));

        qsAndStep.append(addLabeledLineEdits({"Total q orders", "from", "every"}));
        qsAndStep.at(i).at(2)->setText("1.0");
    }

    if(fileNames.size() > 0)
        addThirdButton("Copy to all");
}

bool InputsMFDFA::checkInputs()
{
    for(int i = 0; i < fileNames.size(); i++)
    {
        QString mwStr = winsAndStep.at(i).at(0)->text().trimmed();
        QString MwStr = winsAndStep.at(i).at(1)->text().trimmed();
        QString wsStr = winsAndStep.at(i).at(2)->text().trimmed();
        QString poStr = polOrd.at(i)->text().trimmed();
        QString nqStr = qsAndStep.at(i).at(0)->text().trimmed();
        QString qiStr = qsAndStep.at(i).at(1)->text().trimmed();
        QString qsStr = qsAndStep.at(i).at(2)->text().trimmed();
        if(isCorrectFormat(mwStr) &&
            isCorrectFormat(MwStr) &&
            isCorrectFormat(wsStr) &&
            isCorrectFormat(poStr) &&
            isCorrectFormat(nqStr) &&
            isCorrectFormat(qiStr) &&
            isCorrectFormat(qsStr))
        {
            mw.push_back(mwStr.toInt());
            Mw.push_back(MwStr.toInt());
            ws.push_back(wsStr.toInt());
            po.push_back(poStr.toInt());
            revSeg.at(i)->isChecked() ? rs.push_back(1) : rs.push_back(0);
            nq.push_back(nqStr.toInt());
            qi.push_back(qiStr.toDouble());
            qs.push_back(qsStr.toDouble());
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
    for(int i = 0; i < fileNames.size(); i++)
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
        //number of q orders
        if(nq.at(i) < 1)
            inptErrs.append("- number of q orders must be greater than 0\n");
        //qs step
        if(qs.at(i) <= 0.0)
            inptErrs.append("- step for q orders must be greater than 0\n");
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

void InputsMFDFA::onOkClick()
{
    if(checkInputs())
    {
        std::map<QString, DataFile*> map = dataMap->getDataMap();
        for(int i = 0; i < fileNames.size(); i++)
        {
            std::vector<double> vec = map.at(fileNames.at(i))->getDataOfColumn(columns.at(i).toInt());
            if(mw.at(i) > int(vec.size()))
                mw.at(i) = po.at(i) + 2;
            if(Mw.at(i) > int(vec.size()))
                Mw.at(i) = vec.size();
            MFDFA *fa = new MFDFA(fileNames.at(i).toStdString(), vec, vec.size(),
                               mw.at(i), Mw.at(i), po.at(i), qi.at(i), nq.at(i),
                               ws.at(i), qs.at(i), rs.at(i));
            mfdfa.push_back(fa);
        }

        emit inputsInserted(mfdfa);
        this->close();
    }
}

void InputsMFDFA::onThirdButtonClick()
{
    if(fileNames.size() > 0)
    {
        QString w1 = winsAndStep.at(0).at(0)->text();
        QString w2 = winsAndStep.at(0).at(1)->text();
        QString w3 = winsAndStep.at(0).at(2)->text();
        QString q1 = qsAndStep.at(0).at(0)->text();
        QString q2 = qsAndStep.at(0).at(1)->text();
        QString q3 = qsAndStep.at(0).at(2)->text();
        QString p = polOrd.at(0)->text();
        bool r = revSeg.at(0)->isChecked();

        for(int i = 1; i < int(fileNames.size()); i++)
        {
            winsAndStep.at(i).at(0)->setText(w1);
            winsAndStep.at(i).at(1)->setText(w2);
            winsAndStep.at(i).at(2)->setText(w3);
            qsAndStep.at(i).at(0)->setText(q1);
            qsAndStep.at(i).at(1)->setText(q2);
            qsAndStep.at(i).at(2)->setText(q3);
            polOrd.at(i)->setText(p);
            revSeg.at(i)->setChecked(r);
        }
    }
}
