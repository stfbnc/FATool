#ifndef STARTING_WINDOW_H
#define STARTING_WINDOW_H

#include <QApplication>
#include <QtWidgets>
#include "FAGlobs.h"

class StartingWindow
{
public:
    explicit StartingWindow();
    ~StartingWindow();
private:
    QString getInstrText();
    void onOKClick();
    QCheckBox *cb;
};

#endif // STARTING_WINDOW_H
