#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QWidget>

class QPushButton;
class QCustomPlot;
class MainWindow : public QWidget
{
public:
    explicit MainWindow(QWidget *parent=nullptr);
private:
    void SetDimensions();
    QPushButton *close_button;
    QPushButton *load_button;
    QPushButton *dfa_button;
    QPushButton *mfdfa_button;
    QPushButton *dcca_button;
    QPushButton *ht_button;
    QPushButton *rho_button;
    QCustomPlot *qplot;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
