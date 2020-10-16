#include "base_plot.h"

BasePlot::BasePlot(QWidget *parent) : QCustomPlot(parent)
{
    isLogX = false;
    isLogY = false;
}

BasePlot::~BasePlot(){}

void BasePlot::setBasePlot()
{
    xAxis2->setVisible(true);
    xAxis2->setTickLabels(false);
    yAxis2->setVisible(true);
    yAxis2->setTickLabels(false);
    connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));
    setFont(QFont("sans", fontSmall));
    xAxis2->setLabelFont(QFont("sans", fontBig, QFont::Bold));
    QPen pltPen, gridPen;
    pltPen.setColor(QColor(Qt::black));
    gridPen.setColor(QColor(220, 220, 220));
    gridPen.setStyle(Qt::DashLine);
    xAxis->setBasePen(pltPen);
    xAxis->setTickPen(pltPen);
    xAxis2->setBasePen(pltPen);
    xAxis2->setTickPen(pltPen);
    yAxis->setBasePen(pltPen);
    yAxis->setTickPen(pltPen);
    yAxis2->setBasePen(pltPen);
    yAxis2->setTickPen(pltPen);
    xAxis->grid()->setPen(gridPen);
    yAxis->grid()->setPen(gridPen);
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    textItem = new QLineEdit("", this);
    textItem->setGeometry(0, 0, 0, 0);
    textItem->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    textItem->setFrame(false);
    textItem->setEnabled(false);
    qFont = QFont(font().family(), fontSmall);
    textItem->setFont(qFont);
    textItem->setStyleSheet("color: #000000");
    connect(this, &QCustomPlot::mouseMove, this, &BasePlot::onMouseMove);
}

void BasePlot::setLogAxes(int logAx)
{
    switch (logAx)
    {
    case X:
        isLogX = true;
        isLogY = false;
        break;
    case Y:
        isLogX = false;
        isLogY = true;
        break;
    case XY:
        isLogX = true;
        isLogY = true;
        break;
    default:
        isLogX = false;
        isLogY = false;
        break;
    }
}

void BasePlot::onMouseMove(QMouseEvent *event)
{
    double x = xAxis->pixelToCoord(event->pos().x());
    double y = yAxis->pixelToCoord(event->pos().y());
    if(isLogX)
        x = exp(x);
    if(isLogY)
        y = exp(y);
    QString xStr = QString("%1").arg(x, 5, 'f', 3);
    QString yStr = QString("%1").arg(y, 5, 'f', 3);
    QString text = QString("(%1, %2)").arg(xStr).arg(yStr);
    textItem->setText(text);
    int leftPxl = axisRect()->outerRect().left();
    int bottomPxl = axisRect()->outerRect().bottom();
    int pad = 2;
    QFontMetrics fm(qFont);
    int pixelsWide = fm.horizontalAdvance(text);
    int pixelsHigh = fm.height();
    textItem->setGeometry(leftPxl+pad, bottomPxl-pad-pixelsHigh, pixelsWide+2*pad, pixelsHigh);
}
