#ifndef REFIT_WINDOW_H
#define REFIT_WINDOW_H

#include <QtWidgets>
#include "constants.h"

namespace Ui{
    class RefitDialog;
}

class RefitWindow : public QWidget
{
Q_OBJECT
public:
    explicit RefitWindow(QWidget *parent=nullptr);
    virtual ~RefitWindow();
signals:
    void inputsInserted(int mw, int Mw, int keep, int clear);
private slots:
    void onOKClick();
    void onKeepFitsClick();
    void onClearFitsClick();
private:
    Ui::RefitDialog *ui;
};

#endif // REFIT_WINDOW_H
