#ifndef REFIT_WINDOW_H
#define REFIT_WINDOW_H

#include <QApplication>
#include <QtWidgets>

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
    void SetDimensions();
    QPushButton *ok_button;
    QPushButton *close_button;
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
