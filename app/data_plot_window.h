#ifndef DATA_PLOT_WINDOW_H
#define DATA_PLOT_WINDOW_H

#include "plot_window.h"
#include "data_file.h"
#include "future"

class DataPlotWindow : public PlotWindow
{
Q_OBJECT
public:
    explicit DataPlotWindow(std::vector<DataFile*> dataFile, std::vector<int> cols, QWidget *parent=nullptr);
    ~DataPlotWindow();
//private slots:
//    void onSaveTxtClick() override;
private:
    void plotData() override;

    std::vector<DataFile*> dataFile;
    std::vector<int> cols;
};

#endif // DATA_PLOT_WINDOW_H
