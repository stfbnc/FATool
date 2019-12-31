#ifndef DCCA_WINDOW_H
#define DCCA_WINDOW_H

#include "plot_window.h"
#include "DCCA.h"

class DCCAWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit DCCAWindow(DCCA *dcca_, QWidget *parent=nullptr);
    ~DCCAWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void refitData(int start, int end, double *hSlope, double *hIntcpt) override;
    void plotData() override;

    DCCA *dcca;
};

#endif // DCCA_WINDOW_H
