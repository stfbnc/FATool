#ifndef INPUTS_HT_H
#define INPUTS_HT_H

#include "constants.h"
#include "HT.h"
#include "abstract_inputs_window.h"
#include "files_data.h"

class InputsHT : public AbstractInputsWindow
{
Q_OBJECT
public:
    explicit InputsHT(QStringList fileNames, QStringList columns, FilesData *dataMap, QWidget *parent=nullptr);
    virtual ~InputsHT();
signals:
    void inputsInserted(std::vector<FA*>);
private slots:
    void onOkClick() override;
    void onThirdButtonClick() override;
private:
    void onRangeChkBoxClick(int idx);
    void onStringChkBoxClick(int idx);
    bool checkInputs() override;
    void addWidgets() override;

    QList<QList<QLineEdit*>> scalesAndStep;
    QList<QLineEdit*> customScales;
    QList<QCheckBox*> rangeBox;
    QList<QCheckBox*> stringBox;
    QList<QList<QLineEdit*>> winsAndStep;

    std::vector<FA*> ht = std::vector<FA*>();

    QStringList fileNames;
    QStringList columns;
    FilesData *dataMap;
    std::vector<int> ns = std::vector<int>();
    std::vector<int> ms = std::vector<int>();
    std::vector<int> ss = std::vector<int>();
    QStringList cs;
    std::vector<int> mmw = std::vector<int>();
    std::vector<int> mMw = std::vector<int>();
    std::vector<int> mws = std::vector<int>();
};

#endif // INPUTS_HT_H
