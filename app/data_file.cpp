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
    // aprire il file, leggere riga per riga (se > header),
    // fare lo split in base a del, appendere nella mappa dei dati
    // in base alle colonne che mi servono

    std::string fn = name.toLocal8Bit().constData();
    FileOps fo;
    int len = fo.rowsNumber(fn);
    std::vector<double> t(len), vec(len);
    FILE *f;
    f = fo.openFile(fn, "r");
    if(f){
        for(int j = 0; j < len; j++){
            t[j] = j;
            fscanf(f, "%lf", &vec[j]);
        }
    }
    fclose(f);
}

void DataFile::setNamesAndTypes(std::map<QString, std::pair<QString, QString>> map)
{
    for(auto const& [key, val] : map)
    {
        this->namesMap.emplace(key.toInt(), val.first);
        this->typesMap.emplace(key.toInt(), val.second);
    }
}
