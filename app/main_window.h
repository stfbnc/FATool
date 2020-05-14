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
#include "abstract_inputs_window.h"

class MainWindow : public QWidget
{
Q_OBJECT
    friend class TestClearAll;
    friend class TestInputWin;
    friend class TestLoading;
    friend class TestWidgetExistence;
    friend class TestWinDim;
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

    QPushButton *quitButton = nullptr;
    QPushButton *clearButton = nullptr;
    QPushButton *loadButton = nullptr;
    QPushButton *goButton = nullptr;
    QPushButton *saveButton = nullptr;
    QLabel *analysisLbl = nullptr;
    QComboBox *ddList = nullptr;
    QStringList fileNames;

    InputsDFA *dfaInptWin = nullptr;
    InputsDCCA *dccaInptWin = nullptr;
    InputsMFDFA *mfdfaInptWin = nullptr;
    InputsrhoDCCA *rhodccaInptWin = nullptr;
    InputsHT *htInptWin = nullptr;
    SaveWindow *saveWin = nullptr;
    StartingWindow *startWin = nullptr;

    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
