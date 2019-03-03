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
    int qlen = 7;
    double step = 1.0;

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
    
//    MFDFAsingleQ d = MFDFAsingleQ(fn, mw, Mw, po, q, rv_sg);
//    printf("%d\n", d.getTsLength());
//    printf("%d\n", d.getRangeLength(mw, Mw));
//    d.setFlucVectors();
//    d.winFlucComp();
//    d.H_loglogFit(mw, Mw);
//    printf("%lf | %lf\n", d.getH_intercept(), d.getH());
    
    MFDFA e = MFDFA(fn, mw, Mw, po, qin, qlen, step, rv_sg);
    printf("%d\n", e.getTsLength());
    printf("%d\n", e.getRangeLength(mw, Mw));
    e.setFlucVectors();
    e.getQrange(qin, qlen, step);
    e.winFlucComp();
    e.H_loglogFit(mw, Mw);
    printf("%lf | %lf\n", e.getH_intercept(), e.getH());
    
    return 0;
}
