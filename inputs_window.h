#ifndef INPUTS_WINDOW_H
#define INPUTS_WINDOW_H

#include <QtWidgets>
#include "constants.h"
#include <sys/stat.h>

class InputsWindow : public QWidget
{
Q_OBJECT
public:
    explicit InputsWindow(QWidget *parent=nullptr);
    ~InputsWindow();
private slots:
    virtual void onOKClick();
private:
    void setDimensions();
protected:
    void addLabel(QString lbl, int row);
    void addButtons(int row);
    QLineEdit* addLabeledEditBox(QString txt, int row, int col=0, int isSmall=1);
    void addLabeledEditBoxArray(QStringList txt, QLineEdit **lineEdits, int row, int col=0);
    QCheckBox* addCheckBox(int row);
    QCheckBox* addLabeledCheckBox(QString txt, int row);
    QComboBox* addLabeledComboBox(QString txt, QStringList elements, int row);
    bool isCorrectFormat(QString txt);
    bool checkFileExistence(std::string fn);
    virtual bool checkInputs();
    virtual void setInputsComponents();
    virtual void setAnalysisObj();

    QPushButton *okButton;
    QPushButton *closeButton;

    int xDim;
    int xWidth;
    int yHeight;
    int padX;
    int padY;
};

#endif // INPUTS_WINDOW_H
