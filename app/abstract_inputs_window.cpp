#include "abstract_inputs_window.h"
#include "ui_abstract_inputs_window.h"

// metodi per checkbox+label

AbstractInputsWindow::AbstractInputsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractInputsWindow)
{
    ui->setupUi(this);
    setTitle("Inputs");

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(close()));

    mainWidget = new QWidget();
    vLayout = new QVBoxLayout(mainWidget);
    ui->scrollArea->setWidget(mainWidget);
    ui->scrollArea->setWidgetResizable(true);
    vLayout->setAlignment(Qt::AlignTop);

    addWidgets();

    qDebug() << mainWidget->width();
    qDebug() << mainWidget->height();
    qDebug() << ui->scrollArea->sizeHint();
    setDimension();

    for(int i = 0; i < vLayout->count(); i++)
        qDebug() << vLayout->itemAt(i)->widget();
}

AbstractInputsWindow::~AbstractInputsWindow()
{
    delete ui;
}

void AbstractInputsWindow::setDimension()
{
    int h = ui->scrollArea->sizeHint().height();
    if(h < scrollAreaHeight)
        ui->scrollArea->resize(scrollAreaWidth, h);
}

void AbstractInputsWindow::setTitle(QString title)
{
    setWindowTitle(title);
}

void AbstractInputsWindow::addWidgets()
{
    QVBoxLayout *vl = getVerticalLayout();

    for(int i = 0; i < 2; i++){
        QLabel *file = iLabel("File_" + QString::number(i) + ".txt", true);
        vl->addWidget(file);


        QStringList winSizesText {"Windows from", "to", "every"};
        QWidget *w1 = iLabeledLineEdit(winSizesText);
        vl->addWidget(w1);
        for(int j = 0; j < w1->layout()->count(); j++){
            qDebug() << "--------------------------";
            qDebug() << w1->layout()->itemAt(j)->widget();
        }
    }
}

QVBoxLayout* AbstractInputsWindow::getVerticalLayout()
{
    return vLayout;
}

QLabel* AbstractInputsWindow::iLabel(QString text, bool isBold)
{
    QLabel *label = new QLabel(text);
    int w = label->fontMetrics().boundingRect(label->text()).width() + 10;
    if(isBold)
        label->setStyleSheet("font-weight: bold");
    label->setGeometry(0, 0, w, 25);

    return label;
}

QWidget* AbstractInputsWindow::iLabeledLineEdit(QStringList text, bool isSmall, bool isBold)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *hl = new QHBoxLayout(widget);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    for(QString str : text){
        QLabel *label = new QLabel(str);
        int w = label->fontMetrics().boundingRect(label->text()).width() + 10;
        if(isBold)
            label->setStyleSheet("font-weight: bold");
        label->setFixedSize(w, 20);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        hl->addWidget(label);

        QLineEdit *lineEdit = new QLineEdit();
        if(isSmall)
            lineEdit->setFixedSize(40, 20);
        else
            lineEdit->setFixedSize(120, 20);
        lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        hl->addWidget(lineEdit);
    }

    return widget;
}
