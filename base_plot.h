#ifndef BASE_PLOT_H
#define BASE_PLOT_H

//#include <QApplication>
#include <QWidget>
#include "qcustomplot.h"

class BasePlot : public QCustomPlot
{
public:
    explicit BasePlot(QWidget *parent=nullptr);
    ~BasePlot();
    void SetBasePlot();//QCustomPlot *plt);
};

#endif // BASE_PLOT_H
