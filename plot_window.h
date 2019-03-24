#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include <QApplication>
#include <QWidget>

class QPushButton;
class QCustomPlot;
class PlotWindow : public QWidget
{
public:
    explicit PlotWindow(QWidget *parent=nullptr);
private:
    void SetDimensions();
    QPushButton *close_button;
    QCustomPlot *qplot;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // PLOT_WINDOW_H
