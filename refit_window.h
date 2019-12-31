#ifndef REFIT_WINDOW_H
#define REFIT_WINDOW_H

#include <QtWidgets>
#include "constants.h"

class RefitWindow : public QWidget
{
Q_OBJECT
public:
    explicit RefitWindow(QWidget *parent=nullptr);
    ~RefitWindow();
signals:
    void inputsInserted(int mw, int Mw);
private slots:
    void onOKClick();
private:
    void setDimensions();

    QPushButton *okButton;
    QPushButton *closeButton;
    QLabel *minWin;
    QLabel *maxWin;
    QLineEdit *minWinTxt;
    QLineEdit *maxWinTxt;

    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // REFIT_WINDOW_H
