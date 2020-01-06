#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <QtWidgets>

class LogWindow : public QWidget
{
Q_OBJECT
public:
    explicit LogWindow(QString logs_, QString fileName_, QString fileName2_, QWidget *parent=nullptr);
    ~LogWindow();
private slots:
    void onSave();
private:
    void setDimensions();

    QPushButton *closeBtn;
    QPushButton *saveBtn;
    QTextEdit *txtArea;
    QScrollArea *scrollArea;

    QString fileName;
    QString fileName2;
    QString logs;
    int xDim;
    int yDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // LOG_WINDOW_H
