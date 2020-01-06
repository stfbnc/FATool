#ifndef MASS_EXPONENTS_WINDOW_H
#define MASS_EXPONENTS_WINDOW_H

#include "plot_window.h"
#include "MFDFA.h"

class MassExponentsWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit MassExponentsWindow(MFDFA *mfdfa_, QString fileName_, QWidget *parent=nullptr);
    ~MassExponentsWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    QString fileName;
    MFDFA *mfdfa;
};

#endif // MASS_EXPONENTS_WINDOW_H
