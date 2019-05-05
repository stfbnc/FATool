#ifndef STARTING_WINDOW_H
#define STARTING_WINDOW_H

#include <QApplication>
#include <QtWidgets>

class StartingWindow
{
public:
    explicit StartingWindow();
    ~StartingWindow();
private:
    QString getInstrText();
    void onOKClick();
};

#endif // STARTING_WINDOW_H
