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

    for(int i = 0; i < vLayout->count(); i+=2)
    {
        QString col = qobject_cast<QLabel*>(vLayout->itemAt(i)->widget())->text();
        QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(vLayout->itemAt(i+1)->widget()->layout());
        QString name = qobject_cast<QLineEdit*>(hLayout->itemAt(1)->widget())->text();
        QString type = qobject_cast<QComboBox*>(hLayout->itemAt(2)->widget())->currentText();

        colsMap.emplace(col.split(" ").last(), std::pair<QString, QString>(name, type));
    }
    emit filesSpecsInserted(ui->delEdit->text(), ui->headEdit->text(), colsMap);

    this->close();
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
            QString errToShow = "Columns must be positive numbers.";
            messageBox.critical(nullptr, "Error", errToShow);
            messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
        }
    }

    for(int i = 0; i < colsList.size(); i++)
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
