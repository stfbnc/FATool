#ifndef INPUTS_RHODCCA_H
#define INPUTS_RHODCCA_H

#include "constants.h"
#include "rhoDCCA.h"
#include "abstract_inputs_window.h"
#include "files_data.h"

class InputsrhoDCCA : public AbstractInputsWindow
{
Q_OBJECT
public:
    explicit InputsrhoDCCA(QStringList fileNames, QStringList columns, QWidget *parent=nullptr);
    virtual ~InputsrhoDCCA();
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
    QList<QStringList> fileCouples;
    QList<QStringList> colsCouples;
    QList<QCheckBox*> compThresh;

    std::vector<FA*> rhodcca = std::vector<FA*>();

    QStringList fileNames;
    QStringList columns;
    FilesData *dataMap;
    std::vector<int> mw = std::vector<int>();
    std::vector<int> Mw = std::vector<int>();
    std::vector<int> ws = std::vector<int>();
    std::vector<int> po = std::vector<int>();
    std::vector<bool> ct = std::vector<bool>();
    long long int combs;
};

#endif // INPUTS_RHODCCA_H
