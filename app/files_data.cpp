#include "files_data.h"

FilesData::FilesData()
{
    dataMap = std::map<QString, DataFile*>();
    dataMutex = new std::mutex();
}

FilesData::~FilesData()
{
    for(auto const& [key, val] : dataMap)
        delete dataMap.at(key);

    delete dataMutex;
}

void FilesData::lock()
{
    dataMutex->lock();
}

void FilesData::unlock()
{
    dataMutex->unlock();
}

std::map<QString, DataFile*> FilesData::getDataMap()
{
    return dataMap;
}

void FilesData::setDataMap(QStringList fileNames, QString del, QString header, std::map<QString, std::pair<QString, QString> > fileSpecsMap)
{
    for(int i = 0; i < int(fileNames.size()); i++)
    {
        DataFile *df = new DataFile(fileNames.at(i), del, header.toInt());
        df->setNamesAndTypes(fileSpecsMap);
        df->setData();
        dataMap.emplace(fileNames.at(i), df);
    }
}
