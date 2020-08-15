#include "data_file.h"

DataFile::DataFile(QString name, QString del, int headers)
{
    this->name = name;
    this->del = del;
    this->headers = headers;
}

DataFile::~DataFile(){}

std::vector<double> DataFile::getDataOfColumn(int col)
{
    return fileMap.at(col);
}

QString DataFile::getNameOfColumn(int col)
{
    return namesMap.at(col);
}

QString DataFile::getTypeOfColumn(int col)
{
    return typesMap.at(col);
}

void DataFile::setData()
{
    QFile inputFile(this->name);
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QStringList realColumns;
        for(auto const& [key, val] : typesMap)
            fileMap.emplace(key, std::vector<double>());

        QTextStream in(&inputFile);
        int headCount = 0;
        while (!in.atEnd())
        {
            headCount++;
            if(headCount > this->headers)
            {
                if(!this->del.isEmpty())
                {
                    QStringList elems = in.readLine().split(QRegularExpression(this->del));
                    for(int i = 0; i < int(elems.size()); i++)
                    {
                        if(columns.contains(QString::number(i + 1)))
                        {
                            fileMap.at(i + 1).push_back(elems.at(i).toDouble());
                            realColumns.append(QString::number(i + 1));
                        }
                    }
                }
                else
                {
                    QString elems = in.readLine();
                    if(columns.contains(QString::number(1)))
                    {
                        fileMap.at(1).push_back(elems.toDouble());
                        realColumns.append(QString::number(1));
                    }
                }
                N++;
            }
        }
        inputFile.close();

        for(QString c : columns)
        {
            if(!realColumns.contains(c))
            {
                fileMap.erase(c.toInt());
                namesMap.erase(c.toInt());
                typesMap.erase(c.toInt());
                columns.removeOne(c);
            }
            if(!columns.contains(QString::number(xCol)))
                xCol = 0;
        }
    }
}

void DataFile::setNamesAndTypes(std::map<QString, std::pair<QString, QString>> map)
{
    for(auto const& [key, val] : map)
    {
        if((val.second == xVec) || (val.second == scalesVec))
            xCol = key.toInt();
        columns.append(key);
        this->namesMap.emplace(key.toInt(), val.first);
        this->typesMap.emplace(key.toInt(), val.second);
    }
}

int DataFile::getXAxisColumn()
{
    return xCol;
}

std::vector<int> DataFile::getColumns()
{
    std::vector<int> cols = std::vector<int>();
    for(QString col : columns)
        cols.push_back(col.toInt());

    return cols;
}

QString DataFile::getFileName()
{
    return this->name;
}

int DataFile::getDataLength()
{
    return N;
}
