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

    InputsDFA *dfaInptWin = new InputsDFA(mainWin.fileNames, {"1"});

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

    mainWin.fileNames.clear();
    mainWin.fileNames.append({dataFile_1, dataFile_2});
    std::map<QString, std::pair<QString, QString>> map = std::map<QString, std::pair<QString, QString>>();
    map.emplace("1", std::pair<QString, QString>("", yVec));
    mainWin.dataMap->setDataMap(mainWin.fileNames, "", "0", map);

    InputsDCCA *dccaInptWin = new InputsDCCA(mainWin.fileNames, {"1", "1"});

    QLineEdit *mw = qobject_cast<QLineEdit *>(dccaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(2));
    QVERIFY(mw);
    QLineEdit *MW = qobject_cast<QLineEdit *>(dccaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(4));
    QVERIFY(MW);
    QLineEdit *st = qobject_cast<QLineEdit *>(dccaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(6));
    QVERIFY(st);
    mw->setText("10");
    QVERIFY(mw->text() == "10");
    MW->setText("100");
    QVERIFY(MW->text() == "100");
    st->setText("2");
    QVERIFY(st->text() == "2");

    QLineEdit *po = qobject_cast<QLineEdit *>(dccaInptWin->ui->scrollArea->widget()->children().at(3)->children().at(2));
    QVERIFY(po);
    po->setText("5");
    QVERIFY(po->text() == "5");

    QComboBox *cb = qobject_cast<QComboBox *>(dccaInptWin->ui->scrollArea->widget()->children().at(4)->children().at(2));
    QVERIFY(cb);
    cb->setCurrentText("signed");
    QVERIFY(cb->currentText() == "signed");

    qRegisterMetaType<std::vector<FA*>>("std::vector<FA*>");
    QSignalSpy spy(dccaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)));
    QTest::mouseClick(dccaInptWin->ui->okButton, Qt::LeftButton);
    spy.wait();
    QVERIFY(spy.count() == 1);
}

void GUITest::testHTInputs()
{
    startLine(__func__);

    mainWin.fileNames.clear();
    mainWin.fileNames.append({dataFile_1});
    std::map<QString, std::pair<QString, QString>> map = std::map<QString, std::pair<QString, QString>>();
    map.emplace("1", std::pair<QString, QString>("", yVec));
    mainWin.dataMap->setDataMap(mainWin.fileNames, "", "0", map);

    InputsHT *htInptWin = new InputsHT(mainWin.fileNames, {"1"});

    QCheckBox *cb = qobject_cast<QCheckBox *>(htInptWin->ui->scrollArea->widget()->children().at(2)->children().at(1));
    QVERIFY(cb);
    cb->click();
    QVERIFY(cb->isChecked());
    QLineEdit *s = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(3)->children().at(2));
    QVERIFY(s);
    QLineEdit *si = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(3)->children().at(4));
    QVERIFY(si);
    QLineEdit *ss = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(3)->children().at(6));
    QVERIFY(ss);
    s->setText("3");
    QVERIFY(s->text() == "3");
    si->setText("100");
    QVERIFY(si->text() == "100");
    ss->setText("2");
    QVERIFY(ss->text() == "2");

    QCheckBox *cb2 = qobject_cast<QCheckBox *>(htInptWin->ui->scrollArea->widget()->children().at(4)->children().at(1));
    QVERIFY(cb2);
    cb2->click();
    QVERIFY(cb2->isChecked() && !cb->isChecked());
    QLineEdit *cs = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(5)->children().at(2));
    QVERIFY(cs);
    cs->setText("300");
    QVERIFY(cs->text() == "300");

    QLineEdit *mi = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(6)->children().at(2));
    QVERIFY(mi);
    QLineEdit *mf = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(6)->children().at(4));
    QVERIFY(mf);
    QLineEdit *ms = qobject_cast<QLineEdit *>(htInptWin->ui->scrollArea->widget()->children().at(6)->children().at(6));
    QVERIFY(ms);
    mi->setText("10");
    QVERIFY(mi->text() == "10");
    mf->setText("100");
    QVERIFY(mf->text() == "100");
    ms->setText("2");
    QVERIFY(ms->text() == "2");
}

void GUITest::testMFDFAInputs()
{
    startLine(__func__);

    mainWin.fileNames.clear();
    mainWin.fileNames.append({dataFile_1});
    std::map<QString, std::pair<QString, QString>> map = std::map<QString, std::pair<QString, QString>>();
    map.emplace("1", std::pair<QString, QString>("", yVec));
    mainWin.dataMap->setDataMap(mainWin.fileNames, "", "0", map);

    InputsMFDFA *mfdfaInptWin = new InputsMFDFA(mainWin.fileNames, {"1"});

    QLineEdit *mw = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(2));
    QVERIFY(mw);
    QLineEdit *MW = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(4));
    QVERIFY(MW);
    QLineEdit *st = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(6));
    QVERIFY(st);
    mw->setText("10");
    QVERIFY(mw->text() == "10");
    MW->setText("100");
    QVERIFY(MW->text() == "100");
    st->setText("2");
    QVERIFY(st->text() == "2");

    QLineEdit *po = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(3)->children().at(2));
    QVERIFY(po);
    po->setText("5");
    QVERIFY(po->text() == "5");

    QCheckBox *cb = qobject_cast<QCheckBox *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(4)->children().at(1));
    QVERIFY(cb);
    cb->setChecked(true);
    QVERIFY(cb->isChecked());

    QLineEdit *q = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(5)->children().at(2));
    QVERIFY(mw);
    QLineEdit *qi = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(5)->children().at(4));
    QVERIFY(MW);
    QLineEdit *qs = qobject_cast<QLineEdit *>(mfdfaInptWin->ui->scrollArea->widget()->children().at(5)->children().at(6));
    QVERIFY(st);
    q->setText("3");
    QVERIFY(q->text() == "3");
    qi->setText("-1");
    QVERIFY(qi->text() == "-1");
    qs->setText("2");
    QVERIFY(qs->text() == "2");

    qRegisterMetaType<std::vector<FA*>>("std::vector<FA*>");
    QSignalSpy spy(mfdfaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)));
    QTest::mouseClick(mfdfaInptWin->ui->okButton, Qt::LeftButton);
    spy.wait();
    QVERIFY(spy.count() == 1);
}

void GUITest::testRHODCCAInputs()
{
    startLine(__func__);

    mainWin.fileNames.clear();
    mainWin.fileNames.append({dataFile_1, dataFile_2});
    std::map<QString, std::pair<QString, QString>> map = std::map<QString, std::pair<QString, QString>>();
    map.emplace("1", std::pair<QString, QString>("", yVec));
    mainWin.dataMap->setDataMap(mainWin.fileNames, "", "0", map);

    InputsrhoDCCA *rhodccaInptWin = new InputsrhoDCCA(mainWin.fileNames, {"1", "1"});

    QLineEdit *mw = qobject_cast<QLineEdit *>(rhodccaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(2));
    QVERIFY(mw);
    QLineEdit *MW = qobject_cast<QLineEdit *>(rhodccaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(4));
    QVERIFY(MW);
    QLineEdit *st = qobject_cast<QLineEdit *>(rhodccaInptWin->ui->scrollArea->widget()->children().at(2)->children().at(6));
    QVERIFY(st);
    mw->setText("10");
    QVERIFY(mw->text() == "10");
    MW->setText("100");
    QVERIFY(MW->text() == "100");
    st->setText("2");
    QVERIFY(st->text() == "2");

    QLineEdit *po = qobject_cast<QLineEdit *>(rhodccaInptWin->ui->scrollArea->widget()->children().at(3)->children().at(2));
    QVERIFY(po);
    po->setText("5");
    QVERIFY(po->text() == "5");

    QCheckBox *cb = qobject_cast<QCheckBox *>(rhodccaInptWin->ui->scrollArea->widget()->children().at(4)->children().at(1));
    QVERIFY(cb);
    cb->setChecked(true);
    QVERIFY(cb->isChecked());

    qRegisterMetaType<std::vector<FA*>>("std::vector<FA*>");
    QSignalSpy spy(rhodccaInptWin, SIGNAL(inputsInserted(std::vector<FA*>)));
    QTest::mouseClick(rhodccaInptWin->ui->okButton, Qt::LeftButton);
    spy.wait();
    QVERIFY(spy.count() == 1);
}

QTEST_MAIN(GUITest)
#include "gui_test.moc"
