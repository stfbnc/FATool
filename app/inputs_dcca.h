#ifndef INPUTS_DCCA_H
#define INPUTS_DCCA_H

#include "constants.h"
#include "DCCA.h"
#include "inputs_window.h"

class InputsDCCA : public InputsWindow
{
Q_OBJECT
public:
    explicit InputsDCCA(QStringList fileNames_, QWidget *parent=nullptr);
    ~InputsDCCA();
signals:
    void dccaInputsInserted(DCCA **dcca);
private slots:
    void onOKClick() override;
private:
    bool checkInputs() override;
    void setInputsComponents() override;
    void setAnalysisObj() override;

    QLineEdit ***winsAndStep;
    QLineEdit **polOrd;
    QComboBox **absList;
    QStringList *fileCouples;

    DCCA **dcca;

    QStringList fileNames;
    int *mw;
    int *Mw;
    int *ws;
    int *po;
    std::string *al;
    int numComp;
    long long int combs;
};

#endif // INPUTS_DCCA_H
