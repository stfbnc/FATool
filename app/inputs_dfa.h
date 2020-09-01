#ifndef INPUTS_DFA_H
#define INPUTS_DFA_H

#include "constants.h"
#include "DFA.h"
#include "abstract_inputs_window.h"
#include "files_data.h"
#include "data_file.h"

class InputsDFA : public AbstractInputsWindow
{
Q_OBJECT
    friend class TestInputWin;
public:
    explicit InputsDFA(QStringList fileNames, QStringList columns, FilesData *dataMap, QWidget *parent=nullptr);
    virtual ~InputsDFA();
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

    std::vector<FA*> dfa = std::vector<FA*>();

    QStringList fileNames;
    QStringList columns;
    FilesData *dataMap;
    std::vector<int> mw = std::vector<int>();
    std::vector<int> Mw = std::vector<int>();
    std::vector<int> ws = std::vector<int>();
    std::vector<int> po = std::vector<int>();
    std::vector<int> rs = std::vector<int>();
};

#endif // INPUTS_DFA_H
