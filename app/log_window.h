#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <QtWidgets>

namespace Ui{
    class FitLog;
}

class LogWindow : public QWidget
{
Q_OBJECT
public:
    explicit LogWindow(QString logs, QString fileName, QString fileName2="", QWidget *parent=nullptr);
    virtual ~LogWindow();
private slots:
    void onSave();
private:
    Ui::FitLog *ui;

    QString fileName;
    QString fileName2;
    QString logs;
};

#endif // LOG_WINDOW_H
