#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <limits>
#include "HT.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

HT::HT(string fileName, int inputScale, int totScales, int stepScale, int polOrd)
		: HTsingleScale(fileName, inputScale, polOrd), Nscales(totScales)
{
    checkScalesInputs(totScales, stepScale);
    allocateScalesMemory(totScales, getRangeLength(inputScale, N));
    ArrayOps ao = ArrayOps();
    ao.int_range(scales, totScales, scale, stepScale);
}

HT::~HT(){
	delete[] scales;
	for(int i = 0; i < getRangeLength(scale, N); i++){
		delete[] HTmtx[i];
	}
	delete[] HTmtx;
}

void HT::checkScalesInputs(int numScales, int scStep){
	//windows size
	if(numScales < 1 || numScales > N){
		fprintf(stdout, "ERROR %d: number of scales must be included between 1 and time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(scStep < 1 || (scale+(numScales-1)*scStep) > N){
		fprintf(stdout, "ERROR %d: step must be strictly positive and such that scales are smaller than the time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
}

void HT::allocateScalesMemory(int L1, int L2){
	scales = new int [L1];
	HTmtx = new double* [L2];
	for(int i = 0; i < L2; i++){
		HTmtx[i] = new double [L1];
	}
}

void HT::scalesWinFlucComp(){
	int L = getRangeLength(scale, N);
	for(int i = 0; i < Nscales; i++){
		scale = scales[i];
		int Lscale = getRangeLength(scale, N);
		winFlucComp();
		H_loglogFit(1, 1);
		for(int j = 0; j < Lscale; j++){
			HTmtx[j][i] = Ht[j];
		}
		for(int j = Lscale; j < L; j++){
			HTmtx[j][i] = numeric_limits<double>::quiet_NaN();
		}
	}
}

// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void HT::saveFile(string path_tot){
	FileOps fo = FileOps();
	int L = getRangeLength(scale, N);
	FILE *f;
	f = fo.open_file(path_tot, "w");
	fprintf(f, "#scale ");
	for(int i = 0; i < Nscales; i++){
		i == Nscales-1 ? fprintf(f, "%d\n", scales[i]) : fprintf(f, "%d ", scales[i]);
	}
	for(int i = 0; i < L; i++){
		for(int j = 0; j < Nscales; j++){
			j == Nscales-1 ? fprintf(f, "%lf\n", HTmtx[i][j]) : fprintf(f, "%lf ", HTmtx[i][j]);
		}
	}
	fclose(f);
}
