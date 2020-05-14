#ifndef ABSTRACT_INPUTS_WINDOW_H
#define ABSTRACT_INPUTS_WINDOW_H

#include <QtWidgets>
#include "constants.h"

namespace Ui{
    class AbstractInputsWindow;
}

class AbstractInputsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractInputsWindow(QWidget *parent=nullptr);
    virtual ~AbstractInputsWindow();
protected:
    virtual void setTitle(QString title);
    virtual void addWidgets();
    QVBoxLayout* getVerticalLayout();
    QLabel* iLabel(QString text, bool isBold=false);
    QWidget* iLabeledLineEdit(QStringList text, bool isSmall=true, bool isBold=false);
private:
    void setDimension();

    Ui::AbstractInputsWindow *ui;
    QWidget *mainWidget = nullptr;
    QVBoxLayout *vLayout = nullptr;

    const int scrollAreaWidth = 390;
    const int scrollAreaHeight = 440;
};

#endif // ABSTRACT_INPUTS_WINDOW_H
