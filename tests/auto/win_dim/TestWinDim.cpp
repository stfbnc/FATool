#include "main_window.h"

#include <QtTest>
#include <QDebug>

class TestWinDim : public QObject
{
    Q_OBJECT
    
private slots:
    void testWinDim();
private:
    MainWindow mainWin;
};

//////////////////////////////////////////////////////////////////

void TestWinDim::testWinDim()
{
    QVERIFY2(mainWin.width() == mainWin.xDim, "Wrong width.");
    QVERIFY2(mainWin.height() == mainWin.yDim, "Wrong height.");
}

QTEST_MAIN(TestWinDim)
#include "TestWinDim.moc"
