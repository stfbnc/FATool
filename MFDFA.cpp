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
		: MFDFAsingleQ(fileName, minWin, maxWin, polOrd, qIn, revSeg), startq(qIn), Nq(qLen), stepq(qStep)
{
    allocateQmemory(qLen, getRangeLength(min_win, max_win));
}

MFDFA::~MFDFA(){
	delete[] t;
	delete[] y;
	delete[] s;
	delete[] F;
    delete[] qRange;
    delete[] Hq;
    delete[] H_interceptq;
    for(int i = 0; i < getRangeLength(min_win, max_win); i++){
        delete[] flucMtx[i];
    }
    delete[] flucMtx;
}

void MFDFA::allocateQmemory(int L1, int L2){
    qRange = new double [L1];
    Hq = new double [L1];
    H_interceptq = new double [L1];
    flucMtx = new double* [L2];
    for(int i = 0; i < L2; i++){
    	flucMtx[i] = new double [L1];
    }
}

void MFDFA::getQrange(double start, int len, double step){
    ArrayOps ao = ArrayOps();
    ao.double_range(qRange, len, start, step);
}

void MFDFA::qWinFlucComp(){
	getQrange(startq, Nq, stepq);
	int Lq = getRangeLength(min_win, max_win);
	//double flucMtx[Lq][Nq];
	for(int i = 0; i < Nq; i++){
		q = qRange[i];
		winFlucComp();
		for(int j = 0; j < Lq; j++){
			flucMtx[j][i] = F[j];
		}
		H_loglogFit(min_win, max_win);
		Hq[i] = getH();
		H_interceptq[i] = getH_intercept();
		printf("%lf %lf %lf\n", q, Hq[i], H_interceptq[i]);
	}
}

void MFDFA::saveFile(string path_tot){
	FileOps fo = FileOps();
	int Lq = getRangeLength(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot, "w");
	fprintf(f, "#q ");
	for(int i = 0; i < Lq; i++){
		i == Lq-1 ? fprintf(f, "%lf\n", Hq[i]) : fprintf(f, "%lf ", Hq[i]);
	}
    for(int i = 0; i < Lq; i++){
		fprintf(f, "%d ", s[i]);
		for(int j = 0; j < Nq; j++){
			j == Nq-1 ? fprintf(f, "%lf\n", F[i]) : fprintf(f, "%lf ", F[i]);
		}
	}
    fclose(f);
}

void MFDFA::qsaveFile(string path_tot){
	FileOps fo = FileOps();
	FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < Nq; i++){
		fprintf(f, "%lf %lf %lf\n", qRange[i], Hq[i], H_interceptq[i]);
	}
    fclose(f);
}

/*void MFDFA::plot(){
	
}*/

/*void MFDFA::qplot(){
	
}*/
