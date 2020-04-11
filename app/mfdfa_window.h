#ifndef MFDFA_WINDOW_H
#define MFDFA_WINDOW_H

#include "spectrum_window.h"
#include "mass_exponents_window.h"
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
    void onSpectrumClick() override;
    void onMassExponentsClick() override;

    MFDFA *mfdfa;
    QString fileName;

    SpectrumWindow *spectWin;
    MassExponentsWindow *massExpWin;
};

#endif // MFDFA_WINDOW_H
