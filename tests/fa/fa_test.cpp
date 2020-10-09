#include "DFA.h"
#include "DCCA.h"
#include "HT.h"
#include "MFDFA.h"
#include "rhoDCCA.h"

#include <QtTest>
#include <QDebug>

class FaTest : public QObject
{
Q_OBJECT
private slots:
    void checkData();
    void testDFAoutput();
    void testDCCAoutput();
    void testHToutput();
    void testMFDFAoutput();
    void testRHODCCAoutput();
private:
    void startLine(std::string funName);

    QString dataFile_1 = ":/data/data.txt";
    QString dataFile_2 = ":/data/data_2.txt";
};

//////////////////////////////////////////////////////////////////

void FaTest::startLine(std::string funName)
{
    std::cout << "####### " << funName << "() #######" << std::endl;
}

void FaTest::checkData()
{
    startLine(__func__);

    bool e = QFile::exists(dataFile_1);
    QVERIFY(e);
    bool e_2 = QFile::exists(dataFile_2);
    QVERIFY(e_2);
}

void FaTest::testDFAoutput()
{
    startLine(__func__);
}

void FaTest::testDCCAoutput()
{
    startLine(__func__);
}

void FaTest::testHToutput()
{
    startLine(__func__);
}

void FaTest::testMFDFAoutput()
{
    startLine(__func__);
}

void FaTest::testRHODCCAoutput()
{
    startLine(__func__);
}

QTEST_MAIN(FaTest)
#include "fa_test.moc"
