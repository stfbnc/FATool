#include "FAGlobs.h"
#include "FA.h"
#include "DFA.h"
#include "DCCA.h"
#include "rhoDCCA.h"
#include "MFDFAsingleQ.h"
#include "MFDFA.h"
#include "HTsingleScale.h"
#include "HT.h"

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
	int Mw = 10;
	int po = 1;
	int rv_sg = 1;
    double qin = -3.0;
    int qlen = 7;
    double step = 1.0;
	int scale = 10;
	int Nscales = 2;
	int stepSc = 3;

//	DFA a = DFA(fn, mw, Mw, po, 2, rv_sg);
//    printf("%d\n", a.getTsLength());
//    printf("%d\n", a.getRangeLength(mw, Mw));
//    a.setFlucVectors();
//    a.winFlucComp();
//    a.H_loglogFit(mw, Mw);
//    printf("%lf | %lf\n", a.getH_intercept(), a.getH());
//    a.saveFile(".");
//
//    DCCA b = DCCA(fn, fm, mw, Mw, po, DEFAULT_DCCA, 3);
//    printf("%d\n", b.getTsLength());
//    printf("%d\n", b.getRangeLength(mw, Mw));
//    b.setFlucVectors();
//    b.winFlucComp();
//    b.H_loglogFit(mw, Mw);
//    printf("%lf | %lf\n", b.getH_intercept(), b.getH());
//    b.saveFile(".");
//
//    rhoDCCA c = rhoDCCA(fn, fm, mw, Mw, po, 7);
//    c.computeRho();
//    c.saveFile(".");
    
//    MFDFAsingleQ d = MFDFAsingleQ(fn, mw, Mw, po, 2.0, 7, rv_sg);
//    printf("%d\n", d.getTsLength());
//    printf("%d\n", d.getRangeLength(mw, Mw));
//    d.setFlucVectors();
//    d.winFlucComp();
//    d.H_loglogFit(mw, Mw);
//    printf("%lf | %lf\n", d.getH_intercept(), d.getH());
//    d.saveFile(".");
//
//    MFDFA e = MFDFA(fn, mw, Mw, po, qin, qlen, 7, step, rv_sg);
//    printf("%d\n", e.getTsLength());
//    printf("%d\n", e.getRangeLength(mw, Mw));
//    printf("cia0\n");
//    e.setFlucVectors();
//    printf("cia1\n");
//    e.winFlucComp();
//    printf("cia2\n");
//    e.saveFile(".");
//    printf("cia3\n");
//    e.qsaveFile(".");
//    printf("cia4\n");
    
	HTsingleScale f = HTsingleScale(fn, scale);
    printf("%d\n", f.getTsLength());
    printf("%d\n", f.getRangeLength(scale, f.getTsLength()));
    f.setFlucVectors();
    f.winFlucComp();
    f.H_loglogFit(4, f.getTsLength()/5);
	f.saveFile(".");

	HT g = HT(fn, scale, Nscales, stepSc);
	printf("%d\n", g.getTsLength());
	printf("%d\n", g.getRangeLength(scale, g.getTsLength()));
	g.setFlucVectors();
	g.winFlucComp();
	g.saveFile(".");

	HT h = HT(fn, "10,13");
	printf("%d\n", h.getTsLength());
	printf("%d\n", h.getRangeLength(scale, h.getTsLength()));
	h.setFlucVectors();
	h.winFlucComp();
	h.saveFile(".");

    return 0;
}
