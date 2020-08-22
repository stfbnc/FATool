#ifndef UPDATE_TABLE_WIDGET_H
#define UPDATE_TABLE_WIDGET_H

#include "abstract_inputs_window.h"
#include "constants.h"

class UpdateTableWidget : public AbstractInputsWindow
{
Q_OBJECT
public:
    explicit UpdateTableWidget(QStringList files, QStringList cols, QStringList names, QString title="", QWidget *parent=nullptr);
    virtual ~UpdateTableWidget();
private slots:
    void onOkClick() override;
signals:
    void newTableValues(QStringList, QStringList, QStringList);
private:
    void addWidgets() override;

    QList<QLineEdit *> inputNames;
    QStringList files;
    QStringList cols;
    QStringList oldNames;
    QStringList newNames;
};

#endif // UPDATE_TABLE_WIDGET_H
