#include "main_window.h"

#include <QtTest>
#include <QDebug>

class TestWidgetExistence : public QObject
{
    Q_OBJECT
    
private slots:
    void testWidgetExistence();
private:
    MainWindow mainWin;
};

//////////////////////////////////////////////////////////////////

void TestWidgetExistence::testWidgetExistence()
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

QTEST_MAIN(TestWidgetExistence)
#include "TestWidgetExistence.moc"
