#ifndef INPUTS_DFA_H
#define INPUTS_DFA_H

#include "constants.h"
#include "DFA.h"
#include "inputs_window.h"

class InputsDFA : public InputsWindow
{
Q_OBJECT
    friend class TestInputWin;
public:
    explicit InputsDFA(QStringList fileNames_, QWidget *parent=nullptr);
    virtual ~InputsDFA();
signals:
    void dfaInputsInserted(DFA **dfa);
private slots:
    void onOKClick() override;
private:
    bool checkInputs() override;
    void setInputsComponents() override;
    void setAnalysisObj() override;

    QLineEdit ***winsAndStep;
    QLineEdit **polOrd;
    QCheckBox **revSeg;

    DFA **dfa;

    QStringList fileNames;
    int *mw;
    int *Mw;
    int *ws;
    int *po;
    int *rs;
    int numComp;
};

#endif // INPUTS_DFA_H
