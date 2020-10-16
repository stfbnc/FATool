#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <future>
#include <thread>
#include <chrono>
#include "file_ops.h"
#include "save_window.h"
#include "inputs_dfa.h"
#include "inputs_dcca.h"
#include "inputs_mfdfa.h"
#include "inputs_rhodcca.h"
#include "inputs_ht.h"
#include "results_window.h"
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
    friend class GUITest;
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
    void onCloseInputWin(std::vector<FA*> fa);
    void showResults(FA* fa);
    void onClearClick();
signals:
    void allFilesLoaded();
private:
    void closeEvent(QCloseEvent *event);
    void instrWindow();
    void loadFiles();
    void clearFilesTable();

    Ui::MainWindow *ui;

    QStringList fileNames;
    FilesData *dataMap = nullptr;
};

#endif // MAIN_WINDOW_H
