#ifndef INPUTS_HT_H
#define INPUTS_HT_H

#include "constants.h"
#include "HT.h"
#include "inputs_window.h"

class InputsHT : public InputsWindow
{
Q_OBJECT
public:
    explicit InputsHT(QStringList fileNames_, QWidget *parent=nullptr);
    virtual ~InputsHT();
signals:
    void htInputsInserted(HT **ht);
private slots:
    void onOKClick() override;
private:
    void onRangeChkBoxClick(int idx);
    void onStringChkBoxClick(int idx);
    bool checkInputs() override;
    void setInputsComponents() override;
    void setAnalysisObj() override;

    QLineEdit ***scalesAndStep;
    QLineEdit **customScales;
    QCheckBox **rangeBox;
    QCheckBox **stringBox;
    QLineEdit ***winsAndStep;

    HT **ht;

    QStringList fileNames;
    int *ns;
    int *ms;
    int *ss;
    QString *cs;
    int *mmw;
    int *mMw;
    int *mws;
    int numComp;
};

#endif // INPUTS_HT_H
