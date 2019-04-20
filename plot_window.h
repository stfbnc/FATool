#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "base_plot.h"

//class QPushButton;
//class QCustomPlot;
class PlotWindow : public QWidget
{
public:
    explicit PlotWindow(QString fileName, QString analysisType, QHash<QString, QString> *pHash, QWidget *parent=nullptr);
    ~PlotWindow();
    BasePlot *plt;
private:
    void SetDimensions();
    void PerformAnalysis(QString fileName, QString analysisType, QHash<QString, QString> *pHash);
    QLabel *xlim;
    QLabel *ylim;
    QLabel *title;
    QLabel *xlabel;
    QLabel *ylabel;
    QLabel *legend;
    QTextEdit *xlimTxt;
    QTextEdit *ylimTxt;
    QTextEdit *titleTxt;
    QTextEdit *xlabelTxt;
    QTextEdit *ylabelTxt;
    QTextEdit *legendTxt;
    QPushButton *refit;
    QPushButton *replot;
    QPushButton *save_plot;
    QPushButton *save_txt;
    QPushButton *close_button;
    double H;
    double H_intercept;

    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // PLOT_WINDOW_H
