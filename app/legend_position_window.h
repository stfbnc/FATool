#ifndef LEGEND_POSITION_WINDOW_H
#define LEGEND_POSITION_WINDOW_H

#include <QtWidgets>
#include "qcustomplot.h"
#include "constants.h"

namespace Ui{
    class MoveLegendWindow;
}

class MoveLegendWindow : public QWidget
{
Q_OBJECT
public:
    explicit MoveLegendWindow(QCustomPlot *plt, QWidget *parent=nullptr);
    virtual ~MoveLegendWindow();
private slots:
    void onTopLeftCheckboxClick();
    void onTopRightCheckboxClick();
    void onBottomLeftCheckboxClick();
    void onBottomRightCheckboxClick();
    void onCustomCoordsCheckboxClick();
private:
    void refreshLegend(QCustomPlot *plt);

    Ui::MoveLegendWindow *ui;
    int cbSelected = -1;

    enum
    {
        noCheck = -1,
        topLeft = 0,
        topRight = 1,
        bottomLeft = 2,
        bottomRight = 3,
        custom = 4
    };
};

#endif // LEGEND_POSITION_WINDOW_H
