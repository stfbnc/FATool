#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "base_plot.h"
#include "refit_window.h"
#include "FAGlobs.h"
#include "DFA.h"
#include "DCCA.h"
#include "rhoDCCA.h"
#include "MFDFA.h"
#include "HT.h"
#include "FileOps.h"

class PlotWindow : public QWidget
{
Q_OBJECT
public:
    explicit PlotWindow(QString analysisType, QHash<QString, QString> *pHash, QString fileName, QString fileName2="", QWidget *parent=nullptr);
    ~PlotWindow();
    BasePlot *plt;
private slots:
    void onYorNLegend();
    void onRefitClick();
    void onReplotClick();
    void onSavePlotClick();
    void onSaveTxtClick();
    void newFit(int start, int end);
    void EnableButtons();
private:
    void SetDimensions();
    bool PerformAnalysis(QHash<QString, QString> *pHash, QString fileName, QString fileName2);
    void DisableButtons();
    bool setLogAxesByAnalysis();
    void RefitByAnalysis(int start, int end, double *hSlope, double *hIntcpt);
    void PlotByAnalysis();
    bool DFAanalysis(QHash<QString, QString> *pHash, QString fileName);
    bool MFDFAanalysis(QHash<QString, QString> *pHash, QString fileName);
    bool HTanalysis(QHash<QString, QString> *pHash, QString fileName);
    bool DCCAanalysis(QHash<QString, QString> *pHash, QString fileName, QString fileName2);
    bool rhoDCCAanalysis(QHash<QString, QString> *pHash, QString fileName, QString fileName2);
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
    QString analysis;
    DFA *dfa;
    DCCA *dcca;
    rhoDCCA *rhodcca;
    MFDFA *mfdfa;
    HT *ht;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // PLOT_WINDOW_H
