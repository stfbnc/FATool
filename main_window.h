#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QWidget>
#include "savewindow.h"
#include "base_plot.h"

class QPushButton;
class QLabel;
class QComboBox;
class MainWindow : public QWidget
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
    BasePlot *qplot;
private slots:
    void onLoadClick();
    void onSaveClick();
    void EnableButtons();
private:
    void SetDimensions();
    void FillList();
    void DisableButtons();
    QPushButton *quit_button;
    QPushButton *load_button;
    QPushButton *go_button;
    QPushButton *save_button;
    QLabel *fluct_txt;
    QComboBox *dd_list;
    QStringList fileNames;
    SaveWindow *save_win;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
