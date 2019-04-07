#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "savewindow.h"
#include "base_plot.h"
#include "inputs_window.h"

//class QPushButton;
//class QLabel;
//class QComboBox;
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
private:
    void SetDimensions();
    void initializeParamHash();
    void FillList();
    void DisableButtons();
    void EnableButtons();
    QPushButton *quit_button;
    QPushButton *load_button;
    QPushButton *go_button;
    QPushButton *save_button;
    QLabel *fluct_txt;
    QComboBox *dd_list;
    QStringList fileNames;
    SaveWindow *save_win;
    InputsWindow *inpt_win;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
