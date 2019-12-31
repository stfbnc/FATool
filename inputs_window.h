#ifndef INPUTS_WINDOW_H
#define INPUTS_WINDOW_H

#include <QtWidgets>
#include "constants.h"

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
    QLineEdit* addLabeledEditBox(QString txt, int row, int col=0);
    void addLabeledEditBoxArray(QStringList txt, QLineEdit **lineEdits, int row, int col=0);
    QCheckBox* addCheckBox(int row);
    QCheckBox* addLabeledCheckBox(QString txt, int row);
    QComboBox* addLabeledComboBox(QString txt, QStringList elements, int row);
    bool isCorrectFormat(QString txt);
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
