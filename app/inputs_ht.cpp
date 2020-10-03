#include "inputs_ht.h"

InputsHT::InputsHT(QStringList fileNames, QStringList columns, QWidget *parent) :
    AbstractInputsWindow(strHT + " inputs", parent)
{
    this->fileNames = fileNames;
    this->columns = columns;
    this->dataMap = FilesData::getInstance();

    addWidgets();
}

InputsHT::~InputsHT(){}

void InputsHT::addWidgets()
{
    QStringList labels;
    labels.append({"Total scales", "from", "every"});

    for(int i = 0; i < fileNames.size(); i++)
    {
        addLabel(fileNames.at(i).split("/").last(), true);

        rangeBox.append(addCheckbox());
        connect(rangeBox.at(i), &QCheckBox::clicked, [=](){this->onRangeChkBoxClick(i);});

        scalesAndStep.append(addLabeledLineEdits(labels));
        scalesAndStep.at(i).at(2)->setText("1");
        for(int j = 0; j < labels.size(); j++)
            scalesAndStep.at(i).at(j)->setEnabled(false);

        stringBox.append(addCheckbox());
        connect(stringBox.at(i), &QCheckBox::clicked, [=](){this->onStringChkBoxClick(i);});

        customScales.append(addLabeledLineEdit({"Windows sizes (comma separated)"}));
        customScales.at(i)->setEnabled(false);

        winsAndStep.append(addLabeledLineEdits({"MFDFA windows size from", "to", "every"}));
        winsAndStep.at(i).at(2)->setText("1");
    }

    if(fileNames.size() > 0)
        addThirdButton("Copy to all");
}

void InputsHT::onRangeChkBoxClick(int idx)
{
    if(rangeBox.at(idx)->isChecked())
    {
        scalesAndStep.at(idx).at(0)->setEnabled(true);
        scalesAndStep.at(idx).at(1)->setEnabled(true);
        scalesAndStep.at(idx).at(2)->setEnabled(true);
        stringBox.at(idx)->setChecked(false);
        customScales.at(idx)->setText("");
        customScales.at(idx)->setEnabled(false);
    }
    else
    {
        scalesAndStep.at(idx).at(0)->setText("");
        scalesAndStep.at(idx).at(0)->setEnabled(false);
        scalesAndStep.at(idx).at(1)->setText("");
        scalesAndStep.at(idx).at(1)->setEnabled(false);
        scalesAndStep.at(idx).at(2)->setText("1");
        scalesAndStep.at(idx).at(2)->setEnabled(false);
    }
}

void InputsHT::onStringChkBoxClick(int idx)
{
    if(stringBox.at(idx)->isChecked())
    {
        customScales.at(idx)->setEnabled(true);
        rangeBox.at(idx)->setChecked(false);
        scalesAndStep.at(idx).at(0)->setText("");
        scalesAndStep.at(idx).at(0)->setEnabled(false);
        scalesAndStep.at(idx).at(1)->setText("");
        scalesAndStep.at(idx).at(1)->setEnabled(false);
        scalesAndStep.at(idx).at(2)->setText("1");
        scalesAndStep.at(idx).at(2)->setEnabled(false);
    }
    else
    {
        customScales.at(idx)->setText("");
        customScales.at(idx)->setEnabled(false);
    }
}

bool InputsHT::checkInputs()
{
    int L = fileNames.size();

    for(int i = 0; i < L; i++)
    {
        ns.push_back(0);
        ms.push_back(0);
        ss.push_back(0);
        cs.append("");
    }

    QStringList inptErrs;
    for(int i = 0; i < L; i++)
    {
        bool err = false;
        if(rangeBox.at(i)->isChecked()){
            QString nsStr = scalesAndStep.at(i).at(0)->text().trimmed();
            QString msStr = scalesAndStep.at(i).at(1)->text().trimmed();
            QString ssStr = scalesAndStep.at(i).at(2)->text().trimmed();
            if(!msStr.isEmpty())
            {
                if(isCorrectFormat(nsStr) &&
                    isCorrectFormat(msStr) &&
                    isCorrectFormat(ssStr))
                {
                    ns.at(i) = nsStr.toInt();
                    ms.at(i) = msStr.toInt();
                    ss.at(i) = ssStr.toInt();
                }
                else
                {
                    err = true;
                }
            }
        }
        else if(stringBox.at(i)->isChecked())
        {
            cs.replace(i, customScales.at(i)->text().trimmed());
            if(!cs.at(i).isEmpty())
            {
                for(QString s : cs.at(i).split(QString::fromStdString(strSep)))
                {
                    if(!isCorrectFormat(s.trimmed()))
                    {
                        err = true;
                        break;
                    }
                }
            }
            else
            {
                err = true;
            }
        }
        QString mmwStr = winsAndStep.at(i).at(0)->text().trimmed();
        QString mMwStr = winsAndStep.at(i).at(1)->text().trimmed();
        QString mwsStr = winsAndStep.at(i).at(2)->text().trimmed();
        if(isCorrectFormat(mmwStr) &&
            isCorrectFormat(mMwStr) &&
            isCorrectFormat(mwsStr))
        {
            mmw.push_back(mmwStr.toInt());
            mMw.push_back(mMwStr.toInt());
            mws.push_back(mwsStr.toInt());
        }
        else
        {
            err = true;
        }

        if(err)
        {
            QMessageBox messageBox;
            QString errToShow = "Inputs must be numeric and not null!";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
            return false;
        }

        if(rangeBox.at(i)->isChecked())
        {
            //minimum scale size
            if(ms.at(i) < 3)
                inptErrs.append("- scale must be greater than 2\n");
            //number of scales
            if(ns.at(i) < 1)
                inptErrs.append("- number of scales must be at least 1\n");
            //step for scales
            if(ss.at(i) < 1)
                inptErrs.append("- step must be strictly positive and such that scales are smaller than the time series length\n");
        }
        else if(stringBox.at(i)->isChecked())
        {
            for(QString s : cs.at(i).split(QString::fromStdString(strSep)))
            {
                if(s.toInt() < 3)
                    inptErrs.append("- scale must be greater than 2\n");
            }
        }
        //windows size
        if(mMw.at(i) < mmw.at(i))
            inptErrs.append("- MFDFA biggest scale must be greater than smallest scale\n");
        else if(mmw.at(i) < 3)
            inptErrs.append("- MFDFA smallest scale must be greater than 3\n");
        //windows step
        if(mws.at(i) < 1)
            inptErrs.append("- MFDFA step must be greater than 0\n");
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

void InputsHT::onOkClick()
{
    if(checkInputs())
    {
        std::map<QString, DataFile*> map = dataMap->getDataMap();
        for(int i = 0; i < fileNames.size(); i++)
        {
            std::vector<double> vec = map.at(fileNames.at(i))->getDataOfColumn(columns.at(i).toInt());
            if(ms.at(i) != 0)
            {
                if(ms.at(i) > int(vec.size()))
                    ms.at(i) = 3;
                if(ns.at(i) > int(vec.size()))
                    ns.at(i) = vec.size() - ms.at(i) + 1;
                if((ms.at(i) + (ns.at(i) - 1) * ss.at(i)) > int(vec.size()))
                    ns.at(i) = (vec.size() - ms.at(i)) / ss.at(i) + 1;
                FA *fa = new HT(fileNames.at(i).toStdString(), vec, vec.size(),
                                ms.at(i), ns.at(i), ss.at(i), mmw.at(i),
                                mMw.at(i), mws.at(i));
                ht.push_back(fa);
            }
            else
            {
                FA *fa = new HT(fileNames.at(i).toStdString(), vec, vec.size(),
                                cs.at(i).toStdString(), mmw.at(i), mMw.at(i), mws.at(i));
                ht.push_back(fa);
            }
        }

        emit inputsInserted(ht);
        this->close();
    }
}

void InputsHT::onThirdButtonClick()
{
    if(fileNames.size() > 0)
    {
        QString w1 = winsAndStep.at(0).at(0)->text();
        QString w2 = winsAndStep.at(0).at(1)->text();
        QString w3 = winsAndStep.at(0).at(2)->text();
        QString s1 = scalesAndStep.at(0).at(0)->text();
        bool e1 = scalesAndStep.at(0).at(0)->isEnabled();
        QString s2 = scalesAndStep.at(0).at(1)->text();
        bool e2 = scalesAndStep.at(0).at(1)->isEnabled();
        QString s3 = scalesAndStep.at(0).at(2)->text();
        bool e3 = scalesAndStep.at(0).at(2)->isEnabled();
        QString c = customScales.at(0)->text();
        bool e4 = customScales.at(0)->isEnabled();
        bool r = rangeBox.at(0)->isChecked();
        bool s = stringBox.at(0)->isChecked();

        for(int i = 1; i < int(fileNames.size()); i++)
        {
            winsAndStep.at(i).at(0)->setText(w1);
            winsAndStep.at(i).at(1)->setText(w2);
            winsAndStep.at(i).at(2)->setText(w3);
            scalesAndStep.at(i).at(0)->setText(s1);
            scalesAndStep.at(i).at(0)->setEnabled(e1);
            scalesAndStep.at(i).at(1)->setText(s2);
            scalesAndStep.at(i).at(1)->setEnabled(e2);
            scalesAndStep.at(i).at(2)->setText(s3);
            scalesAndStep.at(i).at(2)->setEnabled(e3);
            customScales.at(i)->setText(c);
            customScales.at(i)->setEnabled(e4);
            rangeBox.at(i)->setChecked(r);
            stringBox.at(i)->setChecked(s);
        }
    }
}
