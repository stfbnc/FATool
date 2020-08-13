#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QVector>
#include <QColor>

//defines for parameters errors
#define RANGE_FAILURE -99
#define WIN_SIZE_FAILURE -98
#define POL_FAILURE -97
#define REV_SEG_FAILURE -96
#define FILE_FAILURE -95
//app version
const QString VERSION = "1.4";
//dcca fluctuation vector
const std::string defaultDCCA = "abs";
const std::string corrDCCA = "sign";
//dimensions
const int xPG = 300;
const int yPG = 120;
const int ERROR_BOX_SIZE = 200;
//preferences file
const QString prefsFile = "prefs.txt";
const QString showStartWin = "Y";
const QString hideStartWin = "N";
//strings
const std::string strSep = ",";
const std::string DCCAfnStart = "DCCA";
const std::string DFAfnStart = "DFA";
const std::string HTsSfnStart = "HTsingleScale";
const std::string HTfnStart = "HT";
const std::string MFDFAsQfnStart = "MFDFAsingleQ";
const std::string MFDFAfnStart = "MFDFA";
const std::string RHODCCAfnStart = "rhoDCCA";
//strings for dropdown list
const QString strDFA = "DFA";
const QString strDCCA = "DCCA";
const QString strHT = "Ht";
const QString strMFDFA = "MFDFA";
const QString strRHODCCA = "rhoDCCA";
//filter for files to analyse
const QString dataFilter = "*.txt *.dat *.csv";
//plots colours
const QVector<QColor> colours = {Qt::red, Qt::blue, Qt::green, Qt::black, Qt::magenta, Qt::cyan};
const QVector<Qt::PenStyle> lineStyles = {Qt::SolidLine, Qt::DashLine, Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine};
//font dimensions
const int fontBig = 14;
const int fontSmall = 12;
//file types
const QString xVec = "x axis";
const QString yVec = "y axis";
const QString flucVec = "fluctuations vector";
const QString scalesVec = "scales vector";

#endif
