#ifndef INPUTS_MFDFA_H
#define INPUTS_MFDFA_H

#include "constants.h"
#include "MFDFA.h"
#include "abstract_inputs_window.h"
#include "files_data.h"

class InputsMFDFA : public AbstractInputsWindow
{
Q_OBJECT
public:
    explicit InputsMFDFA(QStringList fileNames, QStringList columns, QWidget *parent=nullptr);
    virtual ~InputsMFDFA();
signals:
    void inputsInserted(std::vector<FA*>);
private slots:
    void onOkClick() override;
    void onThirdButtonClick() override;
private:
    bool checkInputs() override;
    void addWidgets() override;

    QList<QList<QLineEdit*>> winsAndStep;
    QList<QLineEdit*> polOrd;
    QList<QCheckBox*> revSeg;
    QList<QList<QLineEdit*>> qsAndStep;

    std::vector<FA*> mfdfa = std::vector<FA*>();

    QStringList fileNames;
    QStringList columns;
    std::vector<int> mw = std::vector<int>();
    std::vector<int> Mw = std::vector<int>();
    std::vector<int> ws = std::vector<int>();
    std::vector<int> po = std::vector<int>();
    std::vector<int> rs = std::vector<int>();
    std::vector<int> nq = std::vector<int>();
    std::vector<double> qi = std::vector<double>();
    std::vector<double> qs = std::vector<double>();
};

#endif // INPUTS_MFDFA_H
