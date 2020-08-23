#include "log_window.h"
#include "ui_fit_log.h"

LogWindow::LogWindow(QString logs, QString fileName, QString fileName2, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FitLog)
{
    this->logs = logs;
    this->fileName = fileName;
    this->fileName2 = fileName2;

    ui->setupUi(this);
    if(this->fileName2 != "")
        setWindowTitle("Fits log - " + this->fileName + " & " + this->fileName2);
    else
        setWindowTitle("Fits log - " + this->fileName);
    setFixedSize(this->width(), this->height());

    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));

    ui->txtArea->setText(logs);
}

LogWindow::~LogWindow(){}

void LogWindow::onSave()
{
    QString pathToSave = QFileDialog::getExistingDirectory();
    if((!pathToSave.isNull()) && (!pathToSave.isEmpty()))
    {
        QString pathTot;
        QStringList fnList = fileName.split(".");
        QString fn;
        for(int i = 0; i < fnList.size() - 1; i++)
            fn.append(fnList.at(i));
        pathTot = fn;
        if(fileName2 != "")
        {
            QStringList fnList2 = fileName2.split(".");
            QString fn2;
            for(int i = 0; i < fnList2.size() - 1; i++)
                fn2.append(fnList2.at(i));
            pathTot.append("_" + fn2);
        }
        QFile qFile(pathToSave + "/fitsLog_" + pathTot + ".txt");
        if(qFile.open(QIODevice::WriteOnly))
        {
            QTextStream out(&qFile);
            out << logs;
            qFile.close();
        }
    }

    this->close();
}
