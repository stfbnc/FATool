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
        for(auto const& [key, val] : typesMap)
            fileMap.emplace(key, std::vector<double>());

        QTextStream in(&inputFile);
        int headCount = 0;
        while (!in.atEnd())
        {
            headCount++;
            if(headCount > this->headers)
            {
                QStringList elems = in.readLine().split(this->del);
                for(int i = 0; i < int(elems.size()); i++)
                {
                    if(columns.contains(QString::number(i + 1)))
                        fileMap.at(i + 1).push_back(elems.at(i).toDouble());
                }
            }
        }
        inputFile.close();
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
