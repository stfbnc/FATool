#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QWidget>

class QPushButton;
class QLabel;
class QComboBox;
class QCustomPlot;
class MainWindow : public QWidget
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=nullptr);
private slots:
    void onLoadClick();
private:
    void SetDimensions();
    void FillList();
    //void onLoadClick();
    QPushButton *quit_button;
    QPushButton *load_button;
    QPushButton *go_button;
    QPushButton *save_button;
    QLabel *fluct_txt;
    QComboBox *dd_list;
    QCustomPlot *qplot;
    QStringList fileNames;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // MAIN_WINDOW_H
