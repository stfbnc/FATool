#ifndef LEGEND_POSITION_WINDOW_H
#define LEGEND_POSITION_WINDOW_H

#include <QtWidgets>
#include "qcustomplot.h"
#include "constants.h"

class MoveLegendWindow : public QWidget
{
public:
    explicit MoveLegendWindow(QCustomPlot *plt, QWidget *parent=nullptr);
    ~MoveLegendWindow();
private:
    void setDimensions();
    void onCheckBoxClick(int idx);
    void refreshLegend(QCustomPlot *plt);

    QPushButton *applyButton;
    QPushButton *closeButton;
    QCheckBox **cbs;
    QLineEdit *coordTxt;
    QStringList labels;
    int numCbs;
    int cbSelected;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // LEGEND_POSITION_WINDOW_H
