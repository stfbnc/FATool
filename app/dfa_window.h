#ifndef DFA_WINDOW_H
#define DFA_WINDOW_H

#include "plot_window.h"
#include "DFA.h"

class DFAWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit DFAWindow(DFA *dfa_, QWidget *parent=nullptr);
    virtual ~DFAWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void refitData(int start, int end, double *hSlope, double *hIntcpt) override;
    void plotData() override;

    DFA *dfa;
};

#endif // DFA_WINDOW_H
