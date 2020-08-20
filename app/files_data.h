#ifndef FILES_DATA_H
#define FILES_DATA_H

#include <vector>
#include <map>
#include <mutex>
#include "data_file.h"

class FilesData
{
public:
    explicit FilesData();
    virtual ~FilesData();
    void lock();
    void unlock();
    std::map<QString, DataFile*> getDataMap();
    void setDataMap(QStringList fileNames, QString del, QString header, std::map<QString, std::pair<QString, QString>> fileSpecsMap);
    void updateMap();
    void deleteFromMap(std::vector<std::pair<QString, int>> items);
    void clearMap();
    QStringList getKeys();
private:
    std::mutex *dataMutex;
    std::map<QString, DataFile*> dataMap;
};

#endif // FILES_DATA_H
