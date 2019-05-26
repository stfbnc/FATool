#include "base_plot.h"

BasePlot::BasePlot(bool logVars, QWidget *parent) : QCustomPlot(parent)
{
    isLog = logVars;
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
    textItem = new QLineEdit("", this);
    textItem->setGeometry(0, 0, 0, 0);
    textItem->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    textItem->setFrame(false);
    textItem->setEnabled(false);
    qFont = QFont(font().family(), 12);
    textItem->setFont(qFont);
    textItem->setStyleSheet("color: #000000");
    connect(this, &QCustomPlot::mouseMove, this, &BasePlot::onMouseMove);
}

void BasePlot::onMouseMove(QMouseEvent *event)
{
    double x = xAxis->pixelToCoord(event->pos().x());
    double y = yAxis->pixelToCoord(event->pos().y());
    if(isLog){
        x = exp(x);
        y = exp(y);
    }
    QString xStr, yStr;
    xStr.sprintf("%.3f", x);
    yStr.sprintf("%.3f", y);
    QString text = QString("(%1, %2)").arg(xStr).arg(yStr);
    textItem->setText(text);
    int left_pxl = axisRect()->outerRect().left();
    int bottom_pxl = axisRect()->outerRect().bottom();
    int pad = 2;
    QFontMetrics fm(qFont);
    int pixelsWide = fm.width(text);
    int pixelsHigh = fm.height();
    textItem->setGeometry(left_pxl+pad, bottom_pxl-pad-pixelsHigh, pixelsWide+2*pad, pixelsHigh);
}
