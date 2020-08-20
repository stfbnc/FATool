#include "update_table_widget.h"

UpdateTableWidget::UpdateTableWidget(QStringList files, QStringList cols, QStringList names, QStringList types, QString title, QWidget *parent) :
    AbstractInputsWindow(title, parent)
{
    this->files = files;
    this->cols = cols;
    this->oldNames = names;
    this->oldTypes = types;

    addWidgets();
}

UpdateTableWidget::~UpdateTableWidget(){}

void UpdateTableWidget::onOkClick()
{
    for(int i = 0; i < int(files.size()); i++)
    {
        newNames.append(inputNames.at(i)->text());
        newTypes.append(inputTypes.at(i)->currentText());
    }

    if(!checkInputs())
    {
        QMessageBox messageBox;
        QString errToShow = "There can only be one column representing a x-axis vector, or there cannot be series and fluctuations columns in the same file.";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
    else
    {
        this->hide();
        emit newTableValues(files, cols, newNames, newTypes);
        this->deleteLater();
    }
}

void UpdateTableWidget::addWidgets()
{
    for(int i = 0; i < int(files.size()); i++)
    {
        addLabel(files.at(i) + " (column " + cols.at(i) + ")", true);
        inputNames.append(addLabeledLineEdit({"Name: "}));
        inputTypes.append(addComboBox({yVec, flucVec}));
    }
}

bool UpdateTableWidget::checkInputs()
{
    for(int i = 0; i < int(oldTypes.size()); i++)
    {
        if(((newTypes.at(i) == yVec) && (oldTypes.at(i) == flucVec)) ||
           ((newTypes.at(i) == flucVec) && (oldTypes.at(i) == yVec)))
            return false;
    }

    return true;
}
