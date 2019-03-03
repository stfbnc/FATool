#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "MFDFA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

MFDFA::MFDFA(string fileName, int minWin, int maxWin, int polOrd, double qIn, int qLen, double qStep, int revSeg)
		: MFDFAsingleQ(fileName, minWin, maxWin, polOrd, qIn, revSeg), Nq(qLen), stepq(qStep)
{
    allocateQmemory(qLen);
}

MFDFA::~MFDFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
    delete[] qRange;
    delete[] Hq;
    delete[] H_interceptq;
}

void MFDFA::allocateQmemory(int L){
    qRange = new double [L];
    Hq = new double [L];
    H_interceptq = new double [L];
}

void MFDFA::getQrange(double start, int len, double step){
    ArrayOps ao = ArrayOps();
    ao.double_range(qRange, len, start, step);
    for(int i = 0; i < len; i++)
        printf("%lf\n", qRange[i]);
}

//funzione che fa in loop per i vari q e salva un file unico con le varie fluttuazioni e su prima riga i q

//funzione che salva q, Hq, e intercetta

/*void MFDFA::plot(){
	
}*/
