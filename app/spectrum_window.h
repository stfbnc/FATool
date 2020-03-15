#ifndef SPECTRUM_WINDOW_H
#define SPECTRUM_WINDOW_H

#include "plot_window.h"
#include "MFDFA.h"

class SpectrumWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit SpectrumWindow(MFDFA *mfdfa_, QString fileName_, QWidget *parent=nullptr);
    ~SpectrumWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    QString fileName;
    MFDFA *mfdfa;
};

#endif // SPECTRUM_WINDOW_H
