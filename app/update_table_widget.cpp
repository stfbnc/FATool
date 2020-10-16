#include "update_table_widget.h"

UpdateTableWidget::UpdateTableWidget(QStringList files, QStringList cols, QStringList names, QString title, QWidget *parent) :
    AbstractInputsWindow(title, parent)
{
    this->files = files;
    this->cols = cols;
    this->oldNames = names;

    addWidgets();
}

UpdateTableWidget::~UpdateTableWidget(){}

void UpdateTableWidget::onOkClick()
{
    for(int i = 0; i < int(files.size()); i++)
        newNames.append(inputNames.at(i)->text());

    this->hide();
    emit newTableValues(files, cols, newNames);
    this->deleteLater();
}

void UpdateTableWidget::addWidgets()
{
    for(int i = 0; i < int(files.size()); i++)
    {
        addLabel(files.at(i) + " (column " + cols.at(i) + ")", true);
        QLineEdit *name = addLabeledLineEdit({"Name: "}, false);
        name->setText(oldNames.at(i));
        inputNames.append(name);
    }
}
