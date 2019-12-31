#ifndef MFDFA_WINDOW_H
#define MFDFA_WINDOW_H

#include "plot_window.h"
#include "MFDFA.h"

class MFDFAWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit MFDFAWindow(MFDFA *mfdfa_, QWidget *parent=nullptr);
    ~MFDFAWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    MFDFA *mfdfa;
};

#endif // MFDFA_WINDOW_H
