#include "load_files_window.h"
#include "ui_load_files.h"

LoadFilesWindow::LoadFilesWindow(QString title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadFiles)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->width(), this->height());
    setWindowTitle(title);

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(onOkClick()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClick()));
    connect(ui->colEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

    QWidget *w = new QWidget();
    w->setContentsMargins(0, 0, 0, 0);
    vLayout = new QVBoxLayout(w);
    ui->colsScrollArea->setWidget(w);
    ui->colsScrollArea->setWidgetResizable(true);
    ui->colsScrollArea->setContentsMargins(0, 0, 0, 0);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setContentsMargins(5, 5, 5, 5);
    vLayout->setSpacing(10);
}

LoadFilesWindow::~LoadFilesWindow()
{
    delete ui;
}

void LoadFilesWindow::onCancelClick()
{
    this->close();
}

void LoadFilesWindow::onOkClick()
{
    std::map<QString, std::pair<QString, QString>> colsMap = std::map<QString, std::pair<QString, QString>>();

    bool xVectorFound = false;
    bool xError = false;
    for(int i = 0; i < vLayout->count(); i+=2)
    {
        QString col = qobject_cast<QLabel*>(vLayout->itemAt(i)->widget())->text();
        QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(vLayout->itemAt(i+1)->widget()->layout());
        QString name = qobject_cast<QLineEdit*>(hLayout->itemAt(1)->widget())->text();
        QString type = qobject_cast<QComboBox*>(hLayout->itemAt(2)->widget())->currentText();
        if((type == xVec) || (type == scalesVec))
        {
            if(!xVectorFound)
            {
                xVectorFound = true;
            }
            else
            {
                xError = true;
                break;
            }
        }

        if(!xError)
            colsMap.emplace(col.split(" ").last(), std::pair<QString, QString>(name, type));
    }

    if(!xError)
    {
        QRegularExpression regex("^[0-9]+$");
        if(!regex.match(ui->headEdit->text().trimmed()).hasMatch())
        {
            QMessageBox messageBox;
            QString errToShow = "Header lines must be zero or more.";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
        else if((ui->delEdit->text().isEmpty()) && (colsMap.size() != 1))
        {
            QMessageBox messageBox;
            QString errToShow = "An empty delimiter can only be associated to a single-column file. Column number must be set to 1.";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
        else
        {
            this->hide();
            emit filesSpecsInserted(ui->delEdit->text(), ui->headEdit->text(), colsMap);
            this->deleteLater();
        }
    }
    else
    {
        QMessageBox messageBox;
        QString errToShow = "There can only be one column representing a x-axis vector.";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
}

void LoadFilesWindow::onTextChanged(QString text)
{
    QLayoutItem* item;
    while((item = vLayout->layout()->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    QStringList colsList = text.split(",");
    for(int i = 0; i < colsList.size(); i++)
    {
        QRegularExpression regex("^[0-9]+$");
        if(!regex.match(colsList.at(i).trimmed()).hasMatch() &&
           colsList.at(i).trimmed() != "")
        {
            QMessageBox messageBox;
            QString errToShow = "Column number must be greater than zero.";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
        else
        {
            if(colsList.at(i).trimmed() != "")
            {
                QLabel *label = new QLabel("Column " + colsList.at(i).trimmed());
                int width = label->fontMetrics().boundingRect(label->text()).width() + 10;
                label->setGeometry(0, 0, width, widgetHeight);
                label->setContentsMargins(0, 0, 0, 0);
                vLayout->addWidget(label);

                QWidget *w2 = new QWidget();
                w2->setContentsMargins(0, 0, 0, 0);
                QHBoxLayout *hl = new QHBoxLayout(w2);
                hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                hl->setContentsMargins(10, 0, 0, 0);

                QLabel *label2 = new QLabel("Name: ");
                width = label2->fontMetrics().boundingRect(label2->text()).width() + 10;
                label2->setGeometry(0, 0, width, widgetHeight);
                label2->setContentsMargins(0, 0, 0, 0);
                hl->addWidget(label2);

                QLineEdit *lineEdit = new QLineEdit();
                lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                lineEdit->setContentsMargins(0, 0, 0, 0);
                hl->addWidget(lineEdit);

                QComboBox *comboBox = new QComboBox();
                comboBox->addItems(typesList);
                hl->addWidget(comboBox);

                vLayout->addWidget(w2);
            }
        }
    }
}
