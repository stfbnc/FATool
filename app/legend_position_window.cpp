#include "legend_position_window.h"
#include "ui_move_legend_window.h"

MoveLegendWindow::MoveLegendWindow(QCustomPlot *plt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveLegendWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Legend position");
    this->setFixedSize(this->width(), this->height());

    connect(ui->applyButton, &QPushButton::clicked, [=](){this->refreshLegend(plt);});
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->tlCb, SIGNAL(clicked()), this, SLOT(onTopLeftCheckboxClick()));
    connect(ui->trCb, SIGNAL(clicked()), this, SLOT(onTopRightCheckboxClick()));
    connect(ui->blCb, SIGNAL(clicked()), this, SLOT(onBottomLeftCheckboxClick()));
    connect(ui->brCb, SIGNAL(clicked()), this, SLOT(onBottomRightCheckboxClick()));
    connect(ui->coordCb, SIGNAL(clicked()), this, SLOT(onCustomCoordsCheckboxClick()));

    ui->coordTxt->setEnabled(false);
}

MoveLegendWindow::~MoveLegendWindow()
{
    delete ui;
}

void MoveLegendWindow::onTopLeftCheckboxClick()
{
    ui->trCb->setChecked(false);
    ui->blCb->setChecked(false);
    ui->brCb->setChecked(false);
    ui->coordCb->setChecked(false);
    ui->coordTxt->setEnabled(false);

    if(cbSelected != topLeft)
        cbSelected = topLeft;
    else
        cbSelected = noCheck;
}

void MoveLegendWindow::onTopRightCheckboxClick()
{
    ui->tlCb->setChecked(false);
    ui->blCb->setChecked(false);
    ui->brCb->setChecked(false);
    ui->coordCb->setChecked(false);
    ui->coordTxt->setEnabled(false);

    if(cbSelected != topRight)
        cbSelected = topRight;
    else
        cbSelected = noCheck;
}

void MoveLegendWindow::onBottomLeftCheckboxClick()
{
    ui->tlCb->setChecked(false);
    ui->trCb->setChecked(false);
    ui->brCb->setChecked(false);
    ui->coordCb->setChecked(false);
    ui->coordTxt->setEnabled(false);

    if(cbSelected != bottomLeft)
        cbSelected = bottomLeft;
    else
        cbSelected = noCheck;
}

void MoveLegendWindow::onBottomRightCheckboxClick()
{
    ui->tlCb->setChecked(false);
    ui->trCb->setChecked(false);
    ui->blCb->setChecked(false);
    ui->coordCb->setChecked(false);
    ui->coordTxt->setEnabled(false);

    if(cbSelected != bottomRight)
        cbSelected = bottomRight;
    else
        cbSelected = noCheck;
}

void MoveLegendWindow::onCustomCoordsCheckboxClick()
{
    ui->tlCb->setChecked(false);
    ui->trCb->setChecked(false);
    ui->blCb->setChecked(false);
    ui->brCb->setChecked(false);

    if(cbSelected != custom)
    {
        ui->coordTxt->setEnabled(true);
        cbSelected = custom;
    }
    else
    {
        ui->coordTxt->setEnabled(false);
        cbSelected = noCheck;
    }
}

void MoveLegendWindow::refreshLegend(QCustomPlot *plt)
{
    switch(cbSelected){
        case topLeft:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
            break;
        case topRight:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignTop);
            break;
        case bottomLeft:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
            break;
        case bottomRight:
            plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipBorderAligned);
            plt->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
            break;
        case custom:
            QStringList xl = ui->coordTxt->text().replace(QRegExp("\\s+"), "").split(",");
            QRegExp rgx("^[-]?[0-9]+[.]?[0-9]*$");
            if((xl.size() != 2) || ((!xl.first().contains(rgx)) || (!xl.last().contains(rgx))))
            {
                QMessageBox messageBox;
                QString errToShow = "Input must be numeric and\nmust contain two values!";
                messageBox.critical(nullptr, "Error", errToShow);
                messageBox.setFixedSize(200,200);
            }
            else
            {
                QFont qFont = QFont(font().family(), fontSmall);
                plt->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
                QFontMetrics fm(qFont);
                int numGraphs = plt->itemCount();
                int pixelsWide = 0;
                for(int i = 0; i < numGraphs; i++)
                {
                    int w = fm.width(plt->graph(i)->name());
                    if(w > pixelsWide)
                        pixelsWide = w;
                }
                int pixelsHigh = fm.height() * numGraphs;
                // *** magic *** //
                double xPad = plt->xAxis->pixelToCoord(23) - plt->xAxis->pixelToCoord(10);
                double yPad = plt->yAxis->pixelToCoord(21) - plt->yAxis->pixelToCoord(10);
                double xMin = plt->xAxis->range().lower + xPad;
                double xMax = plt->xAxis->range().upper - xPad;
                double yMin = plt->yAxis->range().lower - yPad;
                double yMax = plt->yAxis->range().upper + yPad;
                double xRange = xMax - xMin;
                double yRange = yMin - yMax;
                QPointF crd((ui->coordTxt->text().split(",").first().trimmed().toDouble() - xMin) / static_cast<double>(xRange),
                            (ui->coordTxt->text().split(",").last().trimmed().toDouble() - yMax) / static_cast<double>(yRange));
                QRectF rect = plt->axisRect()->insetLayout()->insetRect(0);
                rect.moveTopLeft(crd);
                rect.setWidth(pixelsWide);
                rect.setHeight(pixelsHigh);
                plt->axisRect()->insetLayout()->setInsetRect(0, rect);
            }
            break;
    }
    plt->replot();
}
