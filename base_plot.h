#ifndef BASE_PLOT_H
#define BASE_PLOT_H

#include <QWidget>
#include "qcustomplot.h"
#include "FAGlobs.h"

class BasePlot : public QCustomPlot
{
public:
    explicit BasePlot(bool logVars, QWidget *parent=nullptr);
    ~BasePlot();
    void SetBasePlot();
private:
    QFont qFont;
    QLineEdit *textItem;
    bool isLog;
private slots:
    void onMouseMove(QMouseEvent* event);
};

#endif // BASE_PLOT_H
