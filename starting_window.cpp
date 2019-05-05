#include "starting_window.h"

StartingWindow::StartingWindow()
{
    QCheckBox *cb = new QCheckBox("Do not show this again");
    QMessageBox msgbox;
    msgbox.setText(getInstrText());
    msgbox.setIcon(QMessageBox::Icon::Warning);
    msgbox.addButton(QMessageBox::Ok);
    msgbox.setDefaultButton(QMessageBox::Ok);
    msgbox.setCheckBox(cb);
    msgbox.exec();
}

StartingWindow::~StartingWindow(){}

QString StartingWindow::getInstrText()
{
    QString str = "Warnings:\n";
    str.append("\n- data files must have a single column and\n   no header");
    return str;
}

void StartingWindow::onOKClick()
{

}
