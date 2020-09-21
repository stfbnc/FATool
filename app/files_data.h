#ifndef FILES_DATA_H
#define FILES_DATA_H

#include <vector>
#include <map>
#include <mutex>
#include "data_file.h"

class FilesData
{
public:
    static FilesData* getInstance();
    void lock();
    void unlock();
    std::map<QString, DataFile*> getDataMap();
    void setDataMap(QStringList fileNames, QString del, QString header, std::map<QString, std::pair<QString, QString>> fileSpecsMap);
    void deleteFromMap(std::vector<std::pair<QString, int>> items);
    void clearMap();
    QStringList getKeys();
private:
    explicit FilesData();
    virtual ~FilesData();

    static FilesData *instance;
    std::mutex *dataMutex;
    std::map<QString, DataFile*> dataMap = std::map<QString, DataFile*>();
};

#endif // FILES_DATA_H
