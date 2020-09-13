#ifndef MASS_EXPONENTS_WINDOW_H
#define MASS_EXPONENTS_WINDOW_H

#include "plot_window.h"
#include "MFDFA.h"

class MassExponentsWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit MassExponentsWindow(MFDFA *mfdfa, QString fileName, QWidget *parent=nullptr);
    virtual ~MassExponentsWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    MFDFA *mfdfa;
};

#endif // MASS_EXPONENTS_WINDOW_H
