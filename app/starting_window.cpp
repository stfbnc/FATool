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
    switch(ret){
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
    QString str = "FATool (version " + VERSION + ")\nWarnings:\n";
    str.append("\n- windows sizes used in fluctuations computation will be automatically readjusted if they do not comply with time series limits");
    return str;
}

void StartingWindow::onOKClick()
{
    QFile f(prefsFile);
    if(f.open(QFile::WriteOnly)){
        QTextStream stream(&f);
        if(cb->isChecked())
            stream << hideStartWin << endl;
        else
            stream << showStartWin << endl;
        f.close();
    }
}
