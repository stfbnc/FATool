#include "abstract_inputs_window.h"
#include "ui_abstract_inputs_window.h"

AbstractInputsWindow::AbstractInputsWindow(QString title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractInputsWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(this->width(), this->height());
    setWindowTitle(title);

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(onOkClick()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClick()));
    connect(ui->thirdButton, SIGNAL(clicked()), this, SLOT(onThirdButtonClick()));
    connect(ui->allCheckBox, SIGNAL(clicked()), this, SLOT(onBottomCheck()));

    mainWidget = new QWidget();
    mainWidget->setContentsMargins(0, 0, 0, 0);
    vLayout = new QVBoxLayout(mainWidget);
    ui->scrollArea->setWidget(mainWidget);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setContentsMargins(0, 0, 0, 0);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setContentsMargins(5, 5, 5, 5);
    vLayout->setSpacing(10);

    ui->thirdButton->hide();
    ui->allCheckBox->hide();

    //addWidgets();
    setDimension();
}

AbstractInputsWindow::~AbstractInputsWindow()
{
    delete ui;
}

void AbstractInputsWindow::setDimension()
{
    int topMargin, bottomMargin, leftMargin, rightMargin;
    vLayout->getContentsMargins(&leftMargin, &topMargin, &rightMargin, &bottomMargin);
    int hTot = (widgetHeight + vLayout->spacing()) * vLayout->count() -
                vLayout->spacing() + bottomMargin + topMargin;

    if(hTot < ui->scrollArea->height()){
        ui->scrollArea->resize(ui->scrollArea->width(), hTot);
        mainWidget->resize(mainWidget->width(), hTot);

        ui->okButton->setGeometry(ui->okButton->x(), mainWidget->height()+10,
                                  ui->okButton->width(), ui->okButton->height());
        ui->cancelButton->setGeometry(ui->cancelButton->x(), mainWidget->height()+10,
                                      ui->cancelButton->width(), ui->cancelButton->height());
        ui->allCheckBox->setGeometry(ui->allCheckBox->x(), mainWidget->height()+10,
                                     ui->allCheckBox->width(), ui->allCheckBox->height());

        int newHeight = mainWidget->height() + 10 + ui->okButton->height();
        this->setFixedSize(this->width(), newHeight);
    }
}

void AbstractInputsWindow::onOkClick()
{
    this->close();
}

void AbstractInputsWindow::onCancelClick()
{
    this->close();
}

void AbstractInputsWindow::onBottomCheck(){}

void AbstractInputsWindow::onThirdButtonClick(){}

void AbstractInputsWindow::addWidgets()
{
    /*for(int i = 0; i < 6; i++){
        addLabel("File_" + QString::number(i) + ".txt", true);

        QStringList winSizesText {"Windows from", "to", "every"};
        QList<QLineEdit *> winSizes = addLabeledLineEdit(winSizesText);

        QStringList polyText {"Polynomial order"};
        QList<QLineEdit *> polyOrder = addLabeledLineEdit(polyText);

        QCheckBox *backComputation = addCheckbox("Backward computation");
    }

    QCheckBox *bottomCb = addBottomCheckbox("Copy to all");
    QPushButton* btn = addThirdButton("Apply");*/
}

QPushButton* AbstractInputsWindow::getOkButton()
{
    return ui->okButton;
}

QPushButton* AbstractInputsWindow::getCancelButton()
{
    return ui->cancelButton;
}

QPushButton* AbstractInputsWindow::addThirdButton(QString text)
{
    ui->thirdButton->setText(text);
    ui->thirdButton->show();

    return ui->thirdButton;
}

QCheckBox* AbstractInputsWindow::addBottomCheckbox(QString text, bool isBold)
{
    if(isBold)
        ui->allCheckBox->setStyleSheet("font-weight: bold");
    ui->allCheckBox->setText(text);
    ui->allCheckBox->show();

    return ui->allCheckBox;
}

void AbstractInputsWindow::addLabel(QString text, bool isBold)
{
    QLabel *label = new QLabel(text);
    int w = label->fontMetrics().boundingRect(label->text()).width() + 10;
    if(isBold)
        label->setStyleSheet("font-weight: bold");
    label->setGeometry(0, 0, w, widgetHeight);
    label->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(label);
}

QList<QLineEdit *> AbstractInputsWindow::addLabeledLineEdits(QStringList text, bool isSmall, bool isBold)
{
    QWidget *widget = new QWidget();
    widget->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hl = new QHBoxLayout(widget);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hl->setContentsMargins(10, 0, 0, 0);

    QList<QLineEdit *> lineEdits;
    for(QString str : text){
        QLabel *label = new QLabel(str);
        int w = label->fontMetrics().boundingRect(label->text()).width() + 10;
        if(isBold)
            label->setStyleSheet("font-weight: bold");
        label->setFixedSize(w, widgetHeight);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setContentsMargins(0, 0, 0, 0);

        hl->addWidget(label);

        QLineEdit *lineEdit = new QLineEdit();
        if(isSmall)
            lineEdit->setFixedSize(2*widgetHeight, widgetHeight);
        else
            lineEdit->setFixedSize(6*widgetHeight, widgetHeight);
        lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lineEdit->setContentsMargins(0, 0, 0, 0);
        lineEdits.append(lineEdit);

        hl->addWidget(lineEdit);
    }
    vLayout->addWidget(widget);

    return lineEdits;
}

QLineEdit* AbstractInputsWindow::addLabeledLineEdit(QString text, bool isSmall, bool isBold)
{
    QWidget *widget = new QWidget();
    widget->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hl = new QHBoxLayout(widget);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hl->setContentsMargins(10, 0, 0, 0);

    QLabel *label = new QLabel(text);
    int w = label->fontMetrics().boundingRect(label->text()).width() + 10;
    if(isBold)
        label->setStyleSheet("font-weight: bold");
    label->setFixedSize(w, widgetHeight);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setContentsMargins(0, 0, 0, 0);
    hl->addWidget(label);

    QLineEdit *lineEdit = new QLineEdit();
    if(isSmall)
        lineEdit->setFixedSize(2*widgetHeight, widgetHeight);
    else
        lineEdit->setFixedSize(6*widgetHeight, widgetHeight);
    lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lineEdit->setContentsMargins(0, 0, 0, 0);
    hl->addWidget(lineEdit);

    vLayout->addWidget(widget);

    return lineEdit;
}

QCheckBox* AbstractInputsWindow::addCheckbox(QString text, bool isBold)
{
    QWidget *widget = new QWidget();
    widget->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hl = new QHBoxLayout(widget);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hl->setContentsMargins(10, 0, 0, 0);

    QCheckBox *cb = new QCheckBox();
    if(isBold)
        cb->setStyleSheet("font-weight: bold");
    if(text != "")
        cb->setText(text);
    cb->setContentsMargins(10, 0, 0, 0);
    hl->addWidget(cb);

    vLayout->addWidget(widget);

    return cb;
}

QComboBox* AbstractInputsWindow::addComboBox(QStringList list, QString text, bool isBold)
{
    QWidget *widget = new QWidget();
    widget->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hl = new QHBoxLayout(widget);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hl->setContentsMargins(10, 0, 0, 0);

    QLabel *label = new QLabel(text);
    int w = label->fontMetrics().boundingRect(label->text()).width() + 10;
    if(isBold)
        label->setStyleSheet("font-weight: bold");
    label->setFixedSize(w, widgetHeight);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setContentsMargins(0, 0, 0, 0);
    hl->addWidget(label);

    QComboBox *cb = new QComboBox();
    cb->addItems(list);
    cb->setContentsMargins(10, 0, 0, 0);
    hl->addWidget(cb);

    vLayout->addWidget(widget);

    return cb;
}
