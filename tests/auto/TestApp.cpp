#include "main_window.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QtTest>

class TestApp : public QObject
{
    Q_OBJECT
    
    private slots:
    void testWinDim();
    void testLoading();
    void testWidgetExistence();
    /*    void init();
     
     void TestConstruction();
     void TestSize();
     
     void TestClear();
     
     void TestConcat_data();
     void TestConcat();*/
    
private:
    MainWindow mainWin;
};

//////////////////////////////////////////////////////////////////

void TestApp::testWinDim()
{
    QVERIFY2(mainWin.width() == mainWin.xDim, "Wrong width.");
    QVERIFY2(mainWin.height() == mainWin.yDim, "Wrong height.");
}

void TestApp::testLoading()
{
    mainWin.onLoadClick();
    QVERIFY2(mainWin.qplot->graphCount() == 1, "File not loaded.");
}

void TestApp::testWidgetExistence()
{
    QVERIFY2(mainWin.qplot, "Plot area not created.");
    QVERIFY2(mainWin.quitButton, "Quit button not created.");
    QVERIFY2(mainWin.clearButton, "Clear button not created.");
    QVERIFY2(mainWin.loadButton, "Load button not created.");
    QVERIFY2(mainWin.goButton, "Go button not created.");
    QVERIFY2(mainWin.saveButton, "Save button not created.");
    QVERIFY2(mainWin.analysisLbl, "Analysis label not created.");
    QVERIFY2(mainWin.ddList, "Combo box not created.");
}

/*

void TestPanelConcat::TestClear()
{
    // write into input fields
    QTest::keyClicks(panel.mInputA, STR1);
    QTest::keyClicks(panel.mInputB, STR2);

    // click button CONCAT
    QTest::mouseClick(panel.mButtonConcat, Qt::LeftButton);

    // click button CANCEL
    QTest::mouseClick(panel.mButtonCancel, Qt::LeftButton);

    // check all fields are empty
    QVERIFY2(panel.mInputA->text().isEmpty(), "Input A not empty");
    QVERIFY2(panel.mInputB->text().isEmpty(), "Input B not empty");
    QVERIFY2(panel.mLabelRes->text().isEmpty(), "Label result not empty");
}

void TestPanelConcat::TestConcat_data()
{
    QTest::addColumn<QTestEventList>("inputA");
    QTest::addColumn<QTestEventList>("inputB");
    QTest::addColumn<QString>("result");

    QTestEventList listA;
    QTestEventList listB;

    // -- Normal A + B --
    listA.addKeyClicks(STR1);

    listB.addKeyClicks(STR2);

    QTest::newRow("Normal A + B") << listA << listB << STR_RES;

    // -- empty A --
    listA.clear();

    listB.clear();
    listB.addKeyClicks(STR2);

    QTest::newRow("Empty A") << listA << listB << STR2;

    // -- empty B --
    listA.clear();
    listA.addKeyClicks(STR1);

    listB.clear();

    QTest::newRow("Empty B") << listA << listB << STR1;

    // -- all empty --
    listA.clear();

    listB.clear();

    QTest::newRow("All empty") << listA << listB << QString();
}

void TestPanelConcat::TestConcat()
{
    QFETCH(QTestEventList, inputA);
    QFETCH(QTestEventList, inputB);
    QFETCH(QString, result);

    // write into input fields
    inputA.simulate(panel.mInputA);
    inputB.simulate(panel.mInputB);

    // click button CONCAT
    QTest::mouseClick(panel.mButtonConcat, Qt::LeftButton);

    // compare result
    QCOMPARE(panel.mLabelRes->text(), result);
}*/

QTEST_MAIN(TestApp)
#include "TestApp.moc"
