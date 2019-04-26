#ifndef INPUTS_WINDOW_H
#define INPUTS_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "FAGlobs.h"

class InputsWindow : public QWidget
{
Q_OBJECT
public:
    explicit InputsWindow(QString analysisStr, QHash<QString, QString> *pHash, QWidget *parent=nullptr);
    ~InputsWindow();
signals:
    void inputsInserted();
private:
    void SetDimensions();
    void onOKClick(QHash<QString, QString> *pHash);
    bool CheckInputs(QHash<QString, QString> *pHash);
    QPushButton *ok_button;
    QPushButton *close_button;
    QLabel *minWin;
    QLabel *maxWin;
    QLabel *winStep;
    QLabel *polOrd;
    QLabel *revSeg;
    QLabel *isAbs;
    QLineEdit *minWinTxt;
    QLineEdit *maxWinTxt;
    QLineEdit *winStepTxt;
    QLineEdit *polOrdTxt;
    QCheckBox *revSegBox;
    QComboBox *absList;
    QString analysis;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // INPUTS_WINDOW_H
