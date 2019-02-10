#include <iostream>
#include <cmath>
#include "ArrayOps.h"
#include "FileOps.h"
#include "MathOps.h"
#include "DFA.h"
#include "FA.h"

using namespace std;

int main(int argc, char **argv){

	/*string fn = argv[1];
	int mw = stoi(argv[2]);
	int Mw = stoi(argv[3]);
	int po = stoi(argv[4]);
	int rv_sg = stoi(argv[5]);*/
	string fn = "./prova.txt";
	int mw = 4;
	int Mw = 20;
	int po = 1;
	int rv_sg = 1;

	DFA *a = new DFA(fn, mw, Mw, po, rv_sg);
    printf("%d\n", a->GetTsLength());
    printf("%d\n", a->GetNumScales(mw, Mw));
    a->SetFlucVectors();
    a->WinFlucComp();
    printf("%lf\n", a->H_loglogFit(mw, Mw));
    
    return 0;
}
