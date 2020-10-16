#ifndef SPECTRUM_WINDOW_H
#define SPECTRUM_WINDOW_H

#include "plot_window.h"
#include "MFDFA.h"

class SpectrumWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit SpectrumWindow(MFDFA *mfdfa, QString fileName, QWidget *parent=nullptr);
    virtual ~SpectrumWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    MFDFA *mfdfa;
};

#endif // SPECTRUM_WINDOW_H
