#ifndef RESULTS_WINDOW_H
#define RESULTS_WINDOW_H

#include "plot_window.h"
#include "FA.h"
#include "MFDFA.h"
#include "spectrum_window.h"
#include "mass_exponents_window.h"

class ResultsWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit ResultsWindow(FA *fa, QWidget *parent=nullptr);
    virtual ~ResultsWindow();
private slots:
    void onSaveTxtClick() override;
    void onMassExponentsClick() override;
    void onSpectrumClick() override;
private:
    void plotData() override;
    void refitData(int start, int end, double *hSlope, double *hIntcpt) override;

    FA *fa;
};

#endif // RESULTS_WINDOW_H
