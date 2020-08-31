#include "main_window.h"
#include "inputs_dfa.h"
#include "abstract_inputs_window.h"

#include <QtTest>
#include <QDebug>

class TestInputWin : public QObject
{
    Q_OBJECT
/*public:
    explicit TestInputWin();
    ~TestInputWin();*/
private slots:
    void initTestCase();
    void testSimple();
    void testInputWin();
//    void testDFAInputs();
private:
    MainWindow mainWin;
    InputsDFA *inputsDFA;
};

//////////////////////////////////////////////////////////////////

void TestInputWin::initTestCase()
{
    QStringList fn;
    fn.append("/Users/stefano/Desktop/Be-7_UMEA.txt");
    //inputsDFA = new InputsDFA(fn);
}

void TestInputWin::testSimple()
{
    QVERIFY2(0 == 1, "FALLITO!");
}

void TestInputWin::testInputWin()
{
    /*qRegisterMetaType<DFA**>("DFA**");
    qDebug() << "\n";
    qDebug() << "####### Inside testInputWin() #######";
    QTRY_VERIFY2_WITH_TIMEOUT(mainWin.fileNames.size()  > 0, "File not selected.", 10000);
    mainWin.onLoadClick();
    mainWin.ddList->setCurrentText(strDFA);
    qDebug() << "Combo box text: " + mainWin.ddList->currentText();
    mainWin.onGoClick();
    QVERIFY2(mainWin.dfaInptWin, "Input window for DFA not created.");
    qDebug() << "Input window for DFA created.";
    QTest::keyClicks(mainWin.dfaInptWin->winsAndStep[0][0], "10");
    QTest::keyClicks(mainWin.dfaInptWin->winsAndStep[0][1], "50");
    QVERIFY2(mainWin.dfaInptWin->winsAndStep[0][0]->text() == "10", "Min win not set correctly.");
    qDebug() << "Min win set correctly.";
    QVERIFY2(mainWin.dfaInptWin->winsAndStep[0][1]->text() == "50", "Max win not set correctly.");
    qDebug() << "Max win set correctly.";
    InputsDFA *inputsDFA = mainWin.dfaInptWin;
    QSignalSpy spy(inputsDFA, SIGNAL(dfaInputsInserted(DFA**)));
    QTimer::singleShot(0, [inputsDFA](){QTest::mouseClick(inputsDFA->okButton, Qt::LeftButton);});
    spy.wait();
    qDebug() << "Signals emitted: " + QString::number(spy.count());
    QCOMPARE(spy.count(), 1);
    qDebug() << "####### Outside testInputWin() #######";
    qDebug() << "\n";*/
}

/*void TestInputWin::testDFAInputs()
{
    qRegisterMetaType<DFA**>("DFA**");
    qDebug() << "\n";
    qDebug() << "####### Inside testDFAInputs() #######";
    QTest::keyClicks(inputsDFA->winsAndStep[0][0], "10");
    QTest::keyClicks(inputsDFA->winsAndStep[0][1], "50");
    QVERIFY2(inputsDFA->winsAndStep[0][0]->text() == "10", "Min win not set correctly.");
    qDebug() << "Min win set correctly.";
    QVERIFY2(inputsDFA->winsAndStep[0][1]->text() == "50", "Max win not set correctly.");
    qDebug() << "Max win set correctly.";
    qDebug() << inputsDFA->fileNames;
    QSignalSpy spy(inputsDFA, SIGNAL(dfaInputsInserted(DFA**)));
    QTimer::singleShot(0, [&](){QTest::mouseClick(inputsDFA->okButton, Qt::LeftButton);});
    spy.wait();
    qDebug() << "Signals emitted: " + QString::number(spy.count());
    QCOMPARE(spy.count(), 1);
    qDebug() << "####### Outside testDFAInputs() #######";
    qDebug() << "\n";
}*/

QTEST_MAIN(TestInputWin)
#include "TestInputWin.moc"
