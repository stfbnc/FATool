#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <vector>
#include <map>
#include <QString>
#include <file_ops.h>

class DataFile
{
public:
    explicit DataFile(QString name, QString del, int headers);
    virtual ~DataFile();

    std::vector<double> getDataOfColumn(int col);
    QString getNameOfColumn(int col);
    QString getTypeOfColumn(int col);
    void setData();
    void setNamesAndTypes(std::map<QString, std::pair<QString, QString>> map);
private:
    QString name = "";
    QString del = "";
    int headers = 0;
    std::map<int, std::vector<double>> fileMap = std::map<int, std::vector<double>>();
    std::map<int, QString> namesMap = std::map<int, QString>();
    std::map<int, QString> typesMap = std::map<int, QString>();
};

#endif // DATA_FILE_H
