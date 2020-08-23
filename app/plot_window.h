#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include "base_plot.h"
#include "refit_window.h"
#include "legend_position_window.h"
#include "log_window.h"
#include "constants.h"

namespace Ui{
    class PlotWindow;
}

class PlotWindow : public QWidget
{
Q_OBJECT
public:
    explicit PlotWindow(QWidget *parent=nullptr);
    virtual ~PlotWindow();
protected:
    Ui::PlotWindow *ui;
    BasePlot *plt;
    QString fileName;
    QString fileName2;

    void setTitle(QString winTitle);
    void setPlotLimits();
    void addLegend();
    void addFitButtons();
    void addSpectrumButton();
    void addMassExponentsButton();
private slots:
    virtual void onMassExponentsClick();
    virtual void onSpectrumClick();
    void onFitLogClick();
    void onMoveLegendClick();
    void onIsLegendCheck();
    void onRefitClick();
    void onReplotClick();
    void onSavePlotClick();
    virtual void onSaveTxtClick();
    void newFitPlot(int start, int end, int keep, int clear);
    void enableButtons();
private:
    void disableButtons();
    virtual void refitData(int start, int end, double *hSlope, double *hIntcpt);
    virtual void plotData() = 0;

    QString fitLog;
};

#endif // PLOT_WINDOW_H
