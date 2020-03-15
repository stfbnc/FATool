#ifndef INPUTS_MFDFA_H
#define INPUTS_MFDFA_H

#include "constants.h"
#include "MFDFA.h"
#include "inputs_window.h"

class InputsMFDFA : public InputsWindow
{
Q_OBJECT
public:
    explicit InputsMFDFA(QStringList fileNames_, QWidget *parent=nullptr);
    ~InputsMFDFA();
signals:
    void mfdfaInputsInserted(MFDFA **mfdfa);
private slots:
    void onOKClick() override;
private:
    bool checkInputs() override;
    void setInputsComponents() override;
    void setAnalysisObj() override;

    QLineEdit ***winsAndStep;
    QLineEdit **polOrd;
    QCheckBox **revSeg;
    QLineEdit ***qsAndStep;

    MFDFA **mfdfa;

    QStringList fileNames;
    int *mw;
    int *Mw;
    int *ws;
    int *po;
    int *rs;
    int *nq;
    double *qi;
    double *qs;
    int numComp;
};

#endif // INPUTS_MFDFA_H
