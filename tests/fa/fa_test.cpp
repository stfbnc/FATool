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
    std::vector<double> readFile(QString fileName, int header=0);

    QString dataFile_1 = ":/data/data.txt";
    QString dataFile_2 = ":/data/data_2.txt";
};

//////////////////////////////////////////////////////////////////

void FaTest::startLine(std::string funName)
{
    std::cout << "####### " << funName << "() #######" << std::endl;
}

std::vector<double> FaTest::readFile(QString fileName, int header)
{
    std::vector<double> data = std::vector<double>();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    int h = 0;
    while(!in.atEnd())
    {
        h++;
        if(h > header)
        {
            QString line = in.readLine();
            data.push_back(line.toDouble());
        }
    }
    file.close();

    return data;
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

    std::vector<double> data = readFile(dataFile_1);
    DFA *dfa = new DFA(std::string("data"), data, data.size(), 10, 100, 1, 1, 1);
    dfa->setVectors();
    dfa->executeAlgorithm();
    dfa->executeFit(10, 100);
    double H = dfa->getH();
    QVERIFY(H - 0.323509 < 1e-5);
}

void FaTest::testDCCAoutput()
{
    startLine(__func__);

    std::vector<double> data_1 = readFile(dataFile_1);
    std::vector<double> data_2 = readFile(dataFile_2);
    DCCA *dcca = new DCCA(std::string("data_1"), data_1, data_1.size(),
                          std::string("data_2"), data_2, data_2.size(),
                          10, 100, 1, corrDCCA.toStdString(), 1, false);
    dcca->setVectors();
    dcca->executeAlgorithm();
    dcca->executeFit(10, 100);
    double H = dcca->getH();
    QVERIFY(H - 0.55591 < 1e-4);
}

void FaTest::testHToutput()
{
    startLine(__func__);

    std::vector<double> data = readFile(dataFile_1);
    HT *ht = new HT(std::string("data"), data, data.size(), "10", 10, 100, 1);
    ht->setVectors();
    ht->executeAlgorithm();
    QVERIFY(ht->HTmtx.at(0).at(0) - 0.352220 < 1e-5);
    QVERIFY(ht->HTmtx.at(20).at(0) - 0.405415 < 1e-5);
    QVERIFY(ht->HTmtx.at(9991).at(0) - 0.372949 < 1e-5);
}

void FaTest::testMFDFAoutput()
{
    startLine(__func__);

    std::vector<double> data = readFile(dataFile_1);
    MFDFA *mfdfa = new MFDFA(std::string("data"), data, data.size(), 10, 100, 1, -1, 3, 1, 1, 1);
    mfdfa->setVectors();
    mfdfa->executeAlgorithm();
    QVERIFY(mfdfa->Hq.at(0) - 0.378244 < 1e-5);
    QVERIFY(mfdfa->Hq.at(1) - 0.360703 < 1e-5);
    QVERIFY(mfdfa->Hq.at(2) - 0.342305 < 1e-5);
}

void FaTest::testRHODCCAoutput()
{
    startLine(__func__);

    std::vector<double> data_1 = readFile(dataFile_1);
    std::vector<double> data_2 = readFile(dataFile_2);
    rhoDCCA *rhodcca = new rhoDCCA(std::string("data_1"), data_1, data_1.size(),
                                   std::string("data_2"), data_2, data_2.size(),
                                   10, 30, 1, 1, true);
    rhodcca->executeAlgorithm();
    QVERIFY(rhodcca->rho.at(0) - 1.00000 < 1e-5);
    QVERIFY(rhodcca->rho.at(10) - 1.00000 < 1e-5);
    QVERIFY(rhodcca->rho.at(20) - 1.00000 < 1e-5);
}

QTEST_MAIN(FaTest)
#include "fa_test.moc"
