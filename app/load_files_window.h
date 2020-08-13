#ifndef LOAD_FILES_WINDOW_H
#define LOAD_FILES_WINDOW_H

#include <QtWidgets>
#include "constants.h"

namespace Ui{
    class LoadFiles;
}

class LoadFilesWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoadFilesWindow(QString title="", QWidget *parent=nullptr);
    virtual ~LoadFilesWindow();
signals:
    void filesSpecsInserted(QString, QString, std::map<QString, std::pair<QString, QString>>);
public slots:
    void onCancelClick();
    void onOkClick();
    void onTextChanged(QString);
private:
    Ui::LoadFiles *ui;
    QVBoxLayout *vLayout = nullptr;

    const int widgetHeight = 20;
    const QStringList typesList {xVec, yVec, flucVec, scalesVec};
};

#endif // LOAD_FILES_WINDOW_H
