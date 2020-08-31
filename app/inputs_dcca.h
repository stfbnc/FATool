#ifndef INPUTS_DCCA_H
#define INPUTS_DCCA_H

#include "constants.h"
#include "DCCA.h"
#include "abstract_inputs_window.h"
#include "files_data.h"

class InputsDCCA : public AbstractInputsWindow
{
Q_OBJECT
public:
    explicit InputsDCCA(QStringList fileNames, QStringList columns, QWidget *parent=nullptr);
    virtual ~InputsDCCA();
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
    QList<QComboBox*> absList;
    QList<QStringList> fileCouples;
    QList<QStringList> colsCouples;

    std::vector<FA*> dcca = std::vector<FA*>();

    QStringList fileNames;
    QStringList columns;
    std::vector<int> mw = std::vector<int>();
    std::vector<int> Mw = std::vector<int>();
    std::vector<int> ws = std::vector<int>();
    std::vector<int> po = std::vector<int>();
    QStringList al;
    long long int combs;
};

#endif // INPUTS_DCCA_H
