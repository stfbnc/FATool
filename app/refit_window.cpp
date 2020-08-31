#include "refit_window.h"
#include "ui_refit_dialog.h"

RefitWindow::RefitWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RefitDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Fit parameters");
    setFixedSize(this->width(), this->height());

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(onOKClick()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->keepFits, SIGNAL(clicked()), this, SLOT(onKeepFitsClick()));
    connect(ui->clearFits, SIGNAL(clicked()), this, SLOT(onClearFitsClick()));
}

RefitWindow::~RefitWindow()
{
    delete ui;
}

void RefitWindow::onOKClick()
{
    QString win1 = ui->minWinTxt->text().trimmed();
    QString win2 = ui->maxWinTxt->text().trimmed();
    QRegExp rgx("^[0-9]+$");
    if((!win1.isEmpty() && win1.contains(rgx)) &&
       (!win2.isEmpty() && win2.contains(rgx))){
        int k, c;
        ui->keepFits->isChecked() ? k = 1 : k = 0;
        ui->clearFits->isChecked() ? c = 1 : c = 0;
        emit inputsInserted(win1.toInt(), win2.toInt(), k, c);
        close();
    }else{
        QMessageBox messageBox;
        QString errToShow = "Inputs must be numeric and not null!";
        messageBox.critical(nullptr, "Error", errToShow);
        messageBox.setFixedSize(ERROR_BOX_SIZE, ERROR_BOX_SIZE);
    }
}

void RefitWindow::onKeepFitsClick()
{
    ui->clearFits->setChecked(false);
}

void RefitWindow::onClearFitsClick()
{
    ui->keepFits->setChecked(false);
}
