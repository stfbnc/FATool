#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <future>
#include <thread>
#include <chrono>
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
#include "load_files_window.h"
#include "data_file.h"
#include "data_plot_window.h"
#include "files_data.h"
#include "update_table_widget.h"

namespace Ui{
    class MainWindow;
}

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
    virtual ~MainWindow();
private slots:
    void onLoadClick();
    void onFilesSpecsInserted(QString, QString, std::map<QString, std::pair<QString, QString>>);
    void afterAllFilesLoaded();
    void updateFilesTable();
    void onMapReady();
    void openContextMenu(const QPoint&);
    void singlePlot();
    void multiplePlots();
    void deleteRows();
    void updateRows();
    void onTableModified(QStringList, QStringList, QStringList);
    void fillList();
    void onGoClick();
    //void onCloseDFAInputWin(DFA **dfa);
    //void onCloseDCCAInputWin(DCCA **dcca);
    //void onCloseMFDFAInputWin(MFDFA **mfdfa);
    //void onCloseRHODCCAInputWin(rhoDCCA **rhodcca);
    //void onCloseHTInputWin(HT **ht);
    void onCloseDFAInputWin(std::vector<FA*> dfa);
    void onCloseDCCAInputWin(std::vector<FA*> dcca);
    void onCloseMFDFAInputWin(std::vector<FA*> mfdfa);
    void onCloseRHODCCAInputWin(std::vector<FA*> rhodcca);
    void onCloseHTInputWin(std::vector<FA*> ht);
    void onClearClick();
signals:
    void allFilesLoaded();
private:
    void closeEvent(QCloseEvent *event);
    void instrWindow();
    void clearFilesTable();

    QStringList fileNames;
    FilesData *dataMap = nullptr;

    InputsDFA *dfaInptWin = nullptr;
    InputsDCCA *dccaInptWin = nullptr;
    InputsMFDFA *mfdfaInptWin = nullptr;
    InputsrhoDCCA *rhodccaInptWin = nullptr;
    InputsHT *htInptWin = nullptr;
    StartingWindow *startWin = nullptr;

    Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_H
