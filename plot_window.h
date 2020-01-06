#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include "base_plot.h"
#include "refit_window.h"
#include "legend_position_window.h"
#include "log_window.h"

class PlotWindow : public QWidget
{
Q_OBJECT
public:
    explicit PlotWindow(QWidget *parent=nullptr);
    ~PlotWindow();
protected:
    BasePlot *plt;
    QString fileName;
    QString fileName2;

    void setTitle(QString winTitle);
    void addPlotFields();
    void addLegend();
    void addRefitButton();
    void addFitLogButton();
    void addSpectrumButton();
    void addMassExponentsButton();
private slots:
    virtual void onMassExponentsClick();
    virtual void onSpectrumClick();
    void onFitLogClick();
    void onMoveLegendClick();
    void onIsLegendCheck();
    void onRefitClick();
    void onReplotClick();
    void onSavePlotClick();
    virtual void onSaveTxtClick();
    void newFitPlot(int start, int end);
    void enableButtons();
private:
	void addButtons();
    void disableButtons();
    virtual void refitData(int start, int end, double *hSlope, double *hIntcpt);
	virtual void plotData();

	const int xDim = 800;
    const int yDim = 500;
    const int xWidth = 110;
    const int yHeight = 30;
    const int padX = 10;
    const int padY = 10;

    QString fitLog;
    QLabel *xLimLbl;
    QLabel *yLimLbl;
    QLabel *titleLbl;
    QLabel *xLabelLbl;
    QLabel *yLabelLbl;
    QLabel *legendLbl;
    QLabel *isLegendLbl;
    QLineEdit *xLimTxt;
    QLineEdit *yLimTxt;
    QLineEdit *titleTxt;
    QLineEdit *xLabelTxt;
    QLineEdit *yLabelTxt;
    QTextEdit *legendTxt;
    QCheckBox *isLegendBox;

    QPushButton *moveLegendBtn;
    QPushButton *massExpBtn;
    QPushButton *spectBtn;
    QPushButton *fitLogBtn;
    QPushButton *refitBtn;
    QPushButton *replotBtn;
    QPushButton *savePlotBtn;
    QPushButton *saveTxtBtn;
    QPushButton *closeBtn;

    RefitWindow *refitWin;
    MoveLegendWindow *moveLegendWin;
    LogWindow *logWin;
};

#endif // PLOT_WINDOW_H
