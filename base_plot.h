#ifndef BASE_PLOT_H
#define BASE_PLOT_H

#include <QWidget>
#include "qcustomplot.h"
#include "constants.h"

class BasePlot : public QCustomPlot
{
public:
    explicit BasePlot(QWidget *parent=nullptr);
    ~BasePlot();
    void setBasePlot();
    void setLogAxes(int logAx);

    static const int X = 0;
    static const int Y = 1;
    static const int XY = 2;
private:
    QFont qFont;
    QLineEdit *textItem;
    bool isLogX;
    bool isLogY;
private slots:
    void onMouseMove(QMouseEvent* event);
};

#endif // BASE_PLOT_H
