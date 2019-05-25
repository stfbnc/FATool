#include "base_plot.h"

BasePlot::BasePlot(QWidget *parent) : QCustomPlot(parent)
{
}

BasePlot::~BasePlot(){}

void BasePlot::SetBasePlot()
{
    xAxis2->setVisible(true);
    xAxis2->setTickLabels(false);
    yAxis2->setVisible(true);
    yAxis2->setTickLabels(false);
    connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));
    setFont(QFont("sans", 12));
    xAxis2->setLabelFont(QFont("sans", 14, QFont::Bold));
    QPen plt_pen, grid_pen;
    plt_pen.setColor(QColor(Qt::black));
    grid_pen.setColor(QColor(220, 220, 220));
    grid_pen.setStyle(Qt::DashLine);
    xAxis->setBasePen(plt_pen);
    xAxis->setTickPen(plt_pen);
    xAxis2->setBasePen(plt_pen);
    xAxis2->setTickPen(plt_pen);
    yAxis->setBasePen(plt_pen);
    yAxis->setTickPen(plt_pen);
    yAxis2->setBasePen(plt_pen);
    yAxis2->setTickPen(plt_pen);
    xAxis->grid()->setPen(grid_pen);
    yAxis->grid()->setPen(grid_pen);
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    textItem = new QCPItemText(this);
    connect(this, &QCustomPlot::mouseMove, this, &BasePlot::onMouseMove);
}

void BasePlot::onMouseMove(QMouseEvent *event)
{
    double x = xAxis->pixelToCoord(event->pos().x());
    double y = yAxis->pixelToCoord(event->pos().y());
    QString xStr, yStr;
    xStr.sprintf("%.2f", x);
    yStr.sprintf("%.2f", y);
    textItem->setText(QString("(%1, %2)").arg(xStr).arg(yStr));
    qInfo() << axisRect()->outerRect().left();
    qInfo() << axisRect()->outerRect().bottom();
    textItem->position->setCoords(QPointF(axisRect()->outerRect().left()+5, axisRect()->outerRect().bottom()+10));
    textItem->setFont(QFont(font().family(), 14));
    textItem->setLayer("overlay");
    textItem->setClipToAxisRect(false);
    replot();
}
