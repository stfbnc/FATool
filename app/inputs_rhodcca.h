#ifndef INPUTS_RHODCCA_H
#define INPUTS_RHODCCA_H

#include "constants.h"
#include "rhoDCCA.h"
#include "inputs_window.h"

class InputsrhoDCCA : public InputsWindow
{
Q_OBJECT
public:
    explicit InputsrhoDCCA(QStringList fileNames_, QWidget *parent=nullptr);
    ~InputsrhoDCCA();
signals:
    void rhodccaInputsInserted(rhoDCCA **rhodcca);
private slots:
    void onOKClick() override;
private:
    bool checkInputs() override;
    void setInputsComponents() override;
    void setAnalysisObj() override;

    QLineEdit ***winsAndStep;
    QLineEdit **polOrd;
    QStringList *fileCouples;
    QCheckBox **compThresh;

    rhoDCCA **rhodcca;

    QStringList fileNames;
    int *mw;
    int *Mw;
    int *ws;
    int *po;
    bool *ct;
    int numComp;
    long long int combs;
};

#endif // INPUTS_RHODCCA_H
