#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "savewindow.h"
#include "base_plot.h"
#include "inputs_window.h"
#include "plot_window.h"
#include "starting_window.h"

class MainWindow : public QWidget
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
    BasePlot *qplot;
    QHash<QString, QString> *paramHash;
private slots:
    void onLoadClick();
    void onSaveClick();
    void onGoClick();
    void onCloseInputWin();
    void EnableButtons();
    void onClearClick();
private:
    void closeEvent(QCloseEvent *event);
    void SetDimensions();
    void initializeParamHash();
    void InstrWindow();
    void FillList();
    void DisableButtons();
    QPushButton *quit_button;
    QPushButton *clear_button;
    QPushButton *load_button;
    QPushButton *go_button;
    QPushButton *save_button;
    QLabel *fluct_txt;
    QComboBox *dd_list;
    QStringList fileNames;
    SaveWindow *save_win;
    InputsWindow *inpt_win;
    StartingWindow *start_win;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
