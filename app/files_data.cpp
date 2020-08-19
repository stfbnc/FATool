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
        /*if(dataMap.find(fileNames.at(i)) != dataMap.end())
        {
            DataFile *df = dataMap.at(fileNames.at(i));
            for(auto const& [key, val] : fileSpecsMap)
            {
                if(std::find(df->getColumns().begin(), df->getColumns().end(), key.toInt()) != df->getColumns().end())
                {
                    df->setNameOfColumn(key.toInt(), val.first);
                    df->setTypeOfColumn(key.toInt(), val.second);
                }
                else
                {

                }
            }
        }
        else
        {*/
        DataFile *df = new DataFile(fileNames.at(i), del, header.toInt());
        df->setNamesAndTypes(fileSpecsMap);
        df->setData();
        dataMap.emplace(fileNames.at(i), df);
        //}
    }
}

QStringList FilesData::getKeys()
{
    QStringList keyList;
    for(auto const& [key, val] : dataMap)
        keyList.append(key);

    return keyList;
}
