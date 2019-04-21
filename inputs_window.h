#ifndef INPUTS_WINDOW_H
#define INPUTS_WINDOW_H

#include <QApplication>
#include <QtWidgets>

//class QPushButton;
//class QLabel;
//class QTextEdit;
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
    QPushButton *ok_button;
    QPushButton *close_button;
    QLabel *minWin;
    QLabel *maxWin;
    QLabel *winStep;
    QLabel *polOrd;
    QLabel *revSeg;
    QLineEdit *minWinTxt;
    QLineEdit *maxWinTxt;
    QLineEdit *winStepTxt;
    QLineEdit *polOrdTxt;
    QCheckBox *revSegBox;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // INPUTS_WINDOW_H
