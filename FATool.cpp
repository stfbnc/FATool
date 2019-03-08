#include <iostream>
#include <cmath>
#include "ArrayOps.h"
#include "FileOps.h"
#include "MathOps.h"
#include "FA.h"
#include "DFA.h"
#include "DCCA.h"
#include "rhoDCCA.h"
#include "MFDFAsingleQ.h"
#include "MFDFA.h"
#include "HTsingleScale.h"

using namespace std;

int main(int argc, char **argv){

	/*string fn = argv[1];
	int mw = stoi(argv[2]);
	int Mw = stoi(argv[3]);
	int po = stoi(argv[4]);
	int rv_sg = stoi(argv[5]);*/
	string fn = "./prova.txt";
    string fm = "./prova2.txt";
	int mw = 4;
	int Mw = 25;
	int po = 1;
	int rv_sg = 1;
    double qin = -3.0;
    int qlen = 2;
    double step = 1.0;
	int scale = 10;

	DFA a = DFA(fn, mw, Mw, po, rv_sg);
    printf("%d\n", a.getTsLength());
    printf("%d\n", a.getRangeLength(mw, Mw));
    a.setFlucVectors();
    a.winFlucComp();
    a.H_loglogFit(mw, Mw);
    printf("%lf | %lf\n", a.getH_intercept(), a.getH());
    
    /*DCCA b = DCCA(fn, fm, mw, Mw, po);
    printf("%d\n", b.getTsLength());
    printf("%d\n", b.getRangeLength(mw, Mw));
    b.setFlucVectors();
    b.winFlucComp();
    b.H_loglogFit(mw, Mw);
    printf("%lf | %lf\n", b.getH_intercept(), b.getH());*/
    
    /*rhoDCCA c = rhoDCCA(fn, fm, mw, Mw, po);
    c.computeRho();
    c.saveFile("./cppDcca.txt");*/
    
    MFDFAsingleQ d = MFDFAsingleQ(fn, 10, 11, po, 0.0, rv_sg);
    printf("%d\n", d.getTsLength());
    printf("%d\n", d.getRangeLength(10, 11));
    d.setFlucVectors();
    d.winFlucComp();
    d.H_loglogFit(10, 11);
    printf("%lf | %lf\n", d.getH_intercept(), d.getH());
    
//    MFDFA e = MFDFA(fn, mw, Mw, po, qin, qlen, step, rv_sg);
//    printf("%d\n", e.getTsLength());
//    printf("%d\n", e.getRangeLength(mw, Mw));
//    printf("cia0\n");
//    e.setFlucVectors();
//    printf("cia1\n");
//    e.qWinFlucComp();
//    printf("cia2\n");
//    e.saveFile("all_fluc.txt");
//    printf("cia3\n");
//    e.qsaveFile("all_q.txt");
//    printf("cia4\n");
    
	HTsingleScale f = HTsingleScale(fn, scale, po);
    printf("%d\n", f.getTsLength());
    printf("%d\n", f.getRangeLength(scale, f.getTsLength()));
    f.setFlucVectors();
    f.winFlucComp();
    f.H_loglogFit();
	f.saveFile("aHT.txt");

    return 0;
}
