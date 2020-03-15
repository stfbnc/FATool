#ifndef RHODCCA_WINDOW_H
#define RHODCCA_WINDOW_H

#include "plot_window.h"
#include "rhoDCCA.h"

class rhoDCCAWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit rhoDCCAWindow(rhoDCCA *rhodcca_, QWidget *parent=nullptr);
    ~rhoDCCAWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    rhoDCCA *rhodcca;
};

#endif // RHODCCA_WINDOW_H
