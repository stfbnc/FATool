#ifndef SAVEWINDOW_H
#define SAVEWINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "qcustomplot.h"

//class QPushButton;
//class QLabel;
//class QTextEdit;
class SaveWindow : public QWidget
{
public:
    explicit SaveWindow(QCustomPlot *plt, QWidget *parent=nullptr);
    ~SaveWindow();
private:
    void SetDimensions();
    void onApply(QCustomPlot *plt);
    void onSave(QCustomPlot *plt);
    QPushButton *save_button;
    QPushButton *apply_button;
    QPushButton *close_button;
    QLabel *xlim;
    QLabel *ylim;
    QLabel *title;
    QLabel *xlabel;
    QLabel *ylabel;
    QLabel *legend;
    QTextEdit *xlimTxt;
    QTextEdit *ylimTxt;
    QTextEdit *titleTxt;
    QTextEdit *xlabelTxt;
    QTextEdit *ylabelTxt;
    QTextEdit *legendTxt;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // SAVEWINDOW_H
