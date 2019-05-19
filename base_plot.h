#ifndef BASE_PLOT_H
#define BASE_PLOT_H

#include <QWidget>
#include "qcustomplot.h"

class BasePlot : public QCustomPlot
{
public:
    explicit BasePlot(QWidget *parent=nullptr);
    ~BasePlot();
    void SetBasePlot();
};

#endif // BASE_PLOT_H
