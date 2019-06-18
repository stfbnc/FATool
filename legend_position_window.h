#ifndef LEGEND_POSITION_WINDOW_H
#define LEGEND_POSITION_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "qcustomplot.h"
#include "FAGlobs.h"

class MoveLegendWindow : public QWidget
{
public:
    explicit MoveLegendWindow(QCustomPlot *plt, QWidget *parent=nullptr);
    ~MoveLegendWindow();
private:
    void SetDimensions();
    void onCheckBoxClick(int idx);
    void RefreshLegend(QCustomPlot *plt);
    void onOKClick(QCustomPlot *plt);
    QPushButton *ok_button;
    QPushButton *apply_button;
    QPushButton *close_button;
    QCheckBox **cbs;
    QLineEdit *coordTxt;
    QStringList labels;
    int num_cbs;
    int cb_selected;
    bool refreshOK;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // LEGEND_POSITION_WINDOW_H
