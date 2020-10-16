#include "files_data.h"

FilesData* FilesData::instance = nullptr;

FilesData::FilesData()
{
    dataMutex = new std::mutex();
}

FilesData::~FilesData()
{
    for(auto const& [key, val] : dataMap)
        delete dataMap.at(key);

    delete dataMutex;
}

FilesData* FilesData::getInstance()
{
    if(!instance)
        instance = new FilesData();

    return instance;
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

void FilesData::setDataMap(QStringList fileNames, QString del, QString header, std::map<QString, std::pair<QString, QString>> fileSpecsMap)
{
    for(int i = 0; i < int(fileNames.size()); i++)
    {
        DataFile *df = new DataFile(fileNames.at(i), del, header.toInt());
        df->setNamesAndTypes(fileSpecsMap);
        df->setData();
        dataMap.emplace(fileNames.at(i), df);
    }
}

QStringList FilesData::getKeys()
{
    QStringList keyList;
    for(auto const& [key, val] : dataMap)
        keyList.append(key);

    return keyList;
}

void FilesData::deleteFromMap(std::vector<std::pair<QString, int> > items)
{
    for(int i = 0; i < int(items.size()); i++)
    {
        DataFile *df = dataMap.at(items.at(i).first);
        df->removeColumn(items.at(i).second);
        if(df->getColumns().size() == 0)
        {
            delete df;
            dataMap.erase(dataMap.find(items.at(i).first));
        }
    }
}

void FilesData::clearMap()
{
    for(auto const& [key, val] : dataMap)
        delete dataMap.at(key);
    dataMap.clear();
}
