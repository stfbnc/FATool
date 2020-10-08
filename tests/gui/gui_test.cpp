#include "main_window.h"
#include "ui_main_window.h"
#include "load_files_window.h"
#include "ui_load_files.h"
#include "files_data.h"
#include "ui_abstract_inputs_window.h"
#include "inputs_dfa.h"
#include "inputs_dcca.h"
#include "inputs_ht.h"
#include "inputs_mfdfa.h"
#include "inputs_rhodcca.h"

#include <QtTest>
#include <QDebug>

class GUITest : public QObject
{
Q_OBJECT
private slots:
    void checkData();
    void testLoad();
    void testFileTable();
    void testDFAInputs();
    void testDCCAInputs();
    void testHTInputs();
    void testMFDFAInputs();
    void testRHODCCAInputs();
private:
    void startLine(std::string funName);

    MainWindow mainWin;

    QString dataFile_1 = ":/data/data.txt";
    QString dataFile_2 = ":/data/data_2.txt";
};

//////////////////////////////////////////////////////////////////

void GUITest::startLine(std::string funName)
{
    std::cout << "####### " << funName << "() #######" << std::endl;
}

void GUITest::checkData()
{
    startLine(__func__);

    bool e = QFile::exists(dataFile_1);
    QVERIFY(e);
    bool e_2 = QFile::exists(dataFile_2);
    QVERIFY(e_2);
}

void GUITest::testLoad()
{
    startLine(__func__);

    mainWin.fileNames.append({dataFile_1, dataFile_2});
    mainWin.loadFiles();
    QWidget *w = QApplication::activeModalWidget();
    LoadFilesWindow *lfw = qobject_cast<LoadFilesWindow *>(w);
    QVERIFY(lfw);

    QTest::keyClicks(lfw->ui->headEdit, "0");
    QVERIFY(lfw->ui->headEdit->text() == "0");
    QTest::keyClicks(lfw->ui->colEdit, "1");
    QVERIFY(lfw->ui->colEdit->text() == "1");

    QLabel *lbl = qobject_cast<QLabel *>(lfw->ui->colsScrollArea->widget()->children().at(1));
    QVERIFY(lbl);
    QComboBox *cmb = qobject_cast<QComboBox *>(lfw->ui->colsScrollArea->widget()->children().at(2)->children().at(3));
    QVERIFY(cmb);
    cmb->setCurrentText(yVec);
    QVERIFY(cmb->currentText() == yVec);

    qRegisterMetaType<std::map<QString, std::pair<QString, QString>>>("std::map<QString, std::pair<QString, QString>>");
    QSignalSpy spy(lfw, SIGNAL(filesSpecsInserted(QString, QString, std::map<QString, std::pair<QString, QString>>)));
    QTest::mouseClick(lfw->ui->okButton, Qt::LeftButton);
    spy.wait();
    QVERIFY(spy.count() == 1);
}

void GUITest::testFileTable()
{
    startLine(__func__);

    mainWin.fileNames.append({dataFile_1, dataFile_2});
    std::map<QString, std::pair<QString, QString>> map = std::map<QString, std::pair<QString, QString>>();
    map.emplace("1", std::pair<QString, QString>("", yVec));
    mainWin.dataMap->setDataMap(mainWin.fileNames, "", "0", map);
    mainWin.updateFilesTable();
    QVERIFY(mainWin.ui->tableWidget->rowCount() == 2);

    mainWin.clearFilesTable();
    QVERIFY(mainWin.ui->tableWidget->rowCount() == 0);
}

void GUITest::testDFAInputs()
{
    startLine(__func__);

    mainWin.fileNames.clear();
    mainWin.fileNames.append({dataFile_1});
    std::map<QString, std::pair<QString, QString>> map = std::map<QString, std::pair<QString, QString>>();
    map.emplace("1", std::pair<QString, QString>("", yVec));
    mainWin.dataMap->setDataMap(mainWin.fileNames, "", "0", map);

    InputsDFA *dfaInptWin = new InputsDFA(mainWin.fileNames, {"1", "1"});

    QLineEdit *mw = qobject_cast<QLineEdit *>(dfaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(2));
    QVERIFY(mw);
    QLineEdit *MW = qobject_cast<QLineEdit *>(dfaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(4));
    QVERIFY(MW);
    QLineEdit *st = qobject_cast<QLineEdit *>(dfaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(6));
    QVERIFY(st);
    mw->setText("10");
    QVERIFY(mw->text() == "10");
    MW->setText("100");
    QVERIFY(MW->text() == "100");
    st->setText("2");
    QVERIFY(st->text() == "2");

    QLineEdit *po = qobject_cast<QLineEdit *>(dfaInptWin->ui->scrollArea->widget()->children().at(3)->children().at(2));
    QVERIFY(po);
    po->setText("5");
    QVERIFY(po->text() == "5");

    QCheckBox *cb = qobject_cast<QCheckBox *>(dfaInptWin->ui->scrollArea->widget()->children().at(4)->children().at(1));
    QVERIFY(cb);
    cb->setChecked(true);
    QVERIFY(cb->isChecked());

    qRegisterMetaType<std::vector<FA*>>("std::vector<FA*>");
    QSignalSpy spy(dfaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)));
    QTest::mouseClick(dfaInptWin->ui->okButton, Qt::LeftButton);
    spy.wait();
    QVERIFY(spy.count() == 1);
}

void GUITest::testDCCAInputs()
{
    startLine(__func__);
}

void GUITest::testHTInputs()
{
    startLine(__func__);
}

void GUITest::testMFDFAInputs()
{
    startLine(__func__);
}

void GUITest::testRHODCCAInputs()
{
    startLine(__func__);
}

QTEST_MAIN(GUITest)
#include "gui_test.moc"
