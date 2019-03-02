#include <iostream>
#include <cmath>
#include "ArrayOps.h"
#include "FileOps.h"
#include "MathOps.h"
#include "FA.h"
#include "DFA.h"
#include "DCCA.h"

using namespace std;

int main(int argc, char **argv){

	/*string fn = argv[1];
	int mw = stoi(argv[2]);
	int Mw = stoi(argv[3]);
	int po = stoi(argv[4]);
	int rv_sg = stoi(argv[5]);*/
	string fn = "./prova2.txt";
    string fm = "./prova.txt";
	int mw = 4;
	int Mw = 25;
	int po = 1;
	int rv_sg = 1;

	DFA a = DFA(fn, mw, Mw, po, rv_sg);
    printf("%d\n", a.getTsLength());
    printf("%d\n", a.getRangeLength(mw, Mw));
    a.setFlucVectors();
    a.winFlucComp();
    a.H_loglogFit(mw, Mw);
    printf("%lf | %lf\n", a.getH_intercept(), a.getH());
    
    DCCA b = DCCA(fn, fm, mw, Mw, po, "sign");
    printf("%d\n", b.getTsLength());
    printf("%d\n", b.getRangeLength(mw, Mw));
    b.setFlucVectors();
    b.winFlucComp();
    b.H_loglogFit(mw, Mw);
    printf("%lf | %lf\n", b.getH_intercept(), b.getH());
    
    return 0;
}
