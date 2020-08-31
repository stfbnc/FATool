#ifndef ABSTRACT_INPUTS_WINDOW_H
#define ABSTRACT_INPUTS_WINDOW_H

#include <QtWidgets>
//#include "constants.h"

namespace Ui{
    class AbstractInputsWindow;
}

class AbstractInputsWindow : public QWidget
{
Q_OBJECT
public:
    explicit AbstractInputsWindow(QString title="", QWidget *parent=nullptr);
    virtual ~AbstractInputsWindow();
protected slots:
    virtual void onOkClick();
    virtual void onCancelClick();
    virtual void onThirdButtonClick();
    virtual void onBottomCheck();
protected:
    virtual void addWidgets() = 0;
    bool isCorrectFormat(QString txt);
    virtual bool checkInputs();
    QPushButton* getOkButton();
    QPushButton* getCancelButton();
    QPushButton* addThirdButton(QString text);
    QCheckBox* addBottomCheckbox(QString text, bool isBold=false);
    void addLabel(QString text, bool isBold=false);
    QList<QLineEdit*> addLabeledLineEdits(QStringList text, bool isBold=false);
    QLineEdit* addLabeledLineEdit(QString text, bool isBold=false);
    QCheckBox* addCheckbox(QString text="", bool isBold=false);
    QComboBox* addComboBox(QStringList list, QString text="", bool isBold=false);

    Ui::AbstractInputsWindow *ui;
private:
    QWidget *mainWidget = nullptr;
    QVBoxLayout *vLayout = nullptr;
};

#endif // ABSTRACT_INPUTS_WINDOW_H
