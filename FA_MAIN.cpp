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
	string fn = "./prova.txt";
	int mw = 4;
	int Mw = 23;
	int po = 1;
	int rv_sg = 0;

	DFA a = DFA(fn, mw, Mw, po, rv_sg);
    printf("%d\n", a.GetTsLength(fn));
    printf("%d\n", a.GetNumScales(mw, Mw));
    a.SetFlucVectors();
    a.WinFlucComp();
    printf("%lf\n", a.H_loglogFit(mw, Mw));
    
    DCCA *b = new DCCA(fn, fn, mw, Mw, po);
    printf("%d\n", b->GetTsLength(fn));
    printf("%d\n", b->GetNumScales(mw, Mw));
    b->SetFlucVectors();
    b->WinFlucComp();
    printf("%lf\n", b->H_loglogFit(mw, Mw));
    
    return 0;
}
