#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDebug>
#include "file_ops.h"
#include "save_window.h"
#include "inputs_dfa.h"
#include "inputs_dcca.h"
#include "inputs_mfdfa.h"
#include "inputs_rhodcca.h"
#include "inputs_ht.h"
#include "dfa_window.h"
#include "dcca_window.h"
#include "mfdfa_window.h"
#include "rhodcca_window.h"
#include "ht_window.h"
#include "starting_window.h"

class MainWindow : public QWidget
{
Q_OBJECT
    friend class TestApp;
public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
    BasePlot *qplot;
private slots:
    void onLoadClick();
    void onSaveClick();
    void onGoClick();
    void onCloseDFAInputWin(DFA **dfa);
    void onCloseDCCAInputWin(DCCA **dcca);
    void onCloseMFDFAInputWin(MFDFA **mfdfa);
    void onCloseRHODCCAInputWin(rhoDCCA **rhodcca);
    void onCloseHTInputWin(HT **ht);
    void enableButtons();
    void onClearClick();
private:
    void closeEvent(QCloseEvent *event);
    void setDimensions();
    void initializeParamHash();
    void instrWindow();
    void fillList();
    void disableButtons();

    QPushButton *quitButton;
    QPushButton *clearButton;
    QPushButton *loadButton;
    QPushButton *goButton;
    QPushButton *saveButton;
    QLabel *analysisLbl;
    QComboBox *ddList;
    QStringList fileNames;

    InputsDFA *dfaInptWin;
    InputsDCCA *dccaInptWin;
    InputsMFDFA *mfdfaInptWin;
    InputsrhoDCCA *rhodccaInptWin;
    InputsHT *htInptWin;
    SaveWindow *saveWin;
    StartingWindow *startWin;

    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
