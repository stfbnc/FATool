#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "base_plot.h"
#include "refit_window.h"
#include "FAGlobs.h"
#include "DFA.h"

class PlotWindow : public QWidget
{
Q_OBJECT
public:
    explicit PlotWindow(QString fileName, QString analysisType, QHash<QString, QString> *pHash, QWidget *parent=nullptr);
    ~PlotWindow();
    BasePlot *plt;
private slots:
    void onYorNLegend();
    void onRefitClick();
    void onReplotClick();
    void onSavePlotClick();
    void onSaveTxtClick();
    void newFit(int start, int end);
private:
    void SetDimensions();
    void PerformAnalysis(QString fileName, QString analysisType, QHash<QString, QString> *pHash);
    void DisableButtons();
    void EnableButtons();
    QLabel *xlim;
    QLabel *ylim;
    QLabel *title;
    QLabel *xlabel;
    QLabel *ylabel;
    QLabel *legend;
    QLabel *legendYorN;
    QLineEdit *xlimTxt;
    QLineEdit *ylimTxt;
    QLineEdit *titleTxt;
    QLineEdit *xlabelTxt;
    QLineEdit *ylabelTxt;
    QTextEdit *legendTxt;
    QCheckBox *legendYorNBox;
    QPushButton *refit;
    QPushButton *replot;
    QPushButton *save_plot;
    QPushButton *save_txt;
    QPushButton *close_button;
    RefitWindow *refit_win;
    DFA *dfa;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // PLOT_WINDOW_H
