#include "starting_window.h"

StartingWindow::StartingWindow()
{
    cb = new QCheckBox("Do not show this again");
    QMessageBox msgbox;
    msgbox.setText(getInstrText());
    msgbox.setIcon(QMessageBox::Icon::Warning);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.setDefaultButton(QMessageBox::Ok);
    msgbox.setCheckBox(cb);
    int ret = msgbox.exec();
    switch(ret)
    {
        case QMessageBox::Ok:
            onOKClick();
            break;
        default:
            break;
    }
}

StartingWindow::~StartingWindow(){}

QString StartingWindow::getInstrText()
{
    QString str = "FATool (version " + VERSION + ")\n\nWarnings:\n";
    str.append("- windows sizes used in fluctuations computation will be automatically readjusted if they do not comply with time series limits");
    return str;
}

void StartingWindow::onOKClick()
{
    QSettings settings;
    if(cb->isChecked())
        settings.setValue(startWinStr, hideStartWin);
    else
        settings.setValue(startWinStr, showStartWin);
}
