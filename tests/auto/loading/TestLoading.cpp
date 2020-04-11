#include "main_window.h"

#include <QtTest>
#include <QDebug>

class TestLoading : public QObject
{
    Q_OBJECT
    
private slots:
    void testLoading();
private:
    MainWindow mainWin;
};

//////////////////////////////////////////////////////////////////

void TestLoading::testLoading()
{
    mainWin.onLoadClick();
    QVERIFY2(mainWin.qplot->graphCount() == 1, "File not loaded.");
}

QTEST_MAIN(TestLoading)
#include "TestLoading.moc"
