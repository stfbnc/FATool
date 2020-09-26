#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <vector>
#include <map>
#include <QString>
#include "constants.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

class DataFile
{
public:
    explicit DataFile(QString name, QString del, int headers);
    virtual ~DataFile();
    std::vector<double> getDataOfColumn(int col);
    QString getNameOfColumn(int col);
    void setNameOfColumn(int col, QString colName);
    QString getTypeOfColumn(int col);
    void setTypeOfColumn(int col, QString typeName);
    void removeColumn(int col);
    void setData();
    void setNamesAndTypes(std::map<QString, std::pair<QString, QString>> map);
    int getXAxisColumn();
    std::vector<int> getColumns();
    QString getFileName();
    int getDataLength();
private:
    QString name = "";
    QString del = "";
    int headers = 0;
    std::map<int, std::vector<double>> fileMap = std::map<int, std::vector<double>>();
    std::map<int, QString> namesMap = std::map<int, QString>();
    std::map<int, QString> typesMap = std::map<int, QString>();
    QStringList columns;
    int xCol = 0;
    int N = 0;
};

#endif // DATA_FILE_H
