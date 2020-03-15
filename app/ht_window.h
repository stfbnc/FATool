#ifndef HT_WINDOW_H
#define HT_WINDOW_H

#include "plot_window.h"
#include "HT.h"

class HTWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit HTWindow(HT *ht_, QWidget *parent=nullptr);
    ~HTWindow();
private slots:
    void onSaveTxtClick() override;
private:
    void plotData() override;

    HT *ht;
};

#endif // HT_WINDOW_H
