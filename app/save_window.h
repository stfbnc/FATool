#ifndef SAVEWINDOW_H
#define SAVEWINDOW_H

#include <QtWidgets>
#include "constants.h"
#include "qcustomplot.h"

class SaveWindow : public QWidget
{
public:
    explicit SaveWindow(QCustomPlot *plt, QWidget *parent=nullptr);
    virtual ~SaveWindow();
private:
    void setDimensions();
    void onApply(QCustomPlot *plt);
    void onSave(QCustomPlot *plt);

    QPushButton *saveButton;
    QPushButton *applyButton;
    QPushButton *closeButton;
    QLabel *xLimLbl;
    QLabel *yLimLbl;
    QLabel *titleLbl;
    QLabel *xLabelLbl;
    QLabel *yLabelLbl;
    QLabel *legendLbl;
    QLineEdit *xLimTxt;
    QLineEdit *yLimTxt;
    QLineEdit *titleTxt;
    QLineEdit *xLabelTxt;
    QLineEdit *yLabelTxt;
    QTextEdit *legendTxt;

    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // SAVEWINDOW_H
