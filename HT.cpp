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
		: HTsingleScale(fileName, inputScale, polOrd), Nscales(totScales), minScale(inputScale)
{
    checkScalesInputs(totScales, stepScale);
    allocateScalesMemory(totScales, getRangeLength(inputScale, N));
    ArrayOps ao = ArrayOps();
    ao.int_range(scales, totScales, inputScale, stepScale);
}

HT::HT(string fileName, string strScales, int polOrd)
		: HTsingleScale(fileName, stoi(strScales.substr(0, strScales.find_first_of(scalesSep))), polOrd)
{
	Nscales = getNumScales(strScales);
	allocateScalesMemory(Nscales, getRangeLength(minScale, N));
	getScales(strScales);
	MathOps mo = MathOps();
	minScale = mo.vec_min(scales, Nscales);
}

HT::~HT(){
	delete[] scales;
	for(int i = 0; i < getRangeLength(minScale, N); i++){
		delete[] HTmtx[i];
	}
	delete[] HTmtx;
}

void HT::checkScalesInputs(int numScales, int scStep){
	//windows size
	if(numScales < 1 || numScales > N){
		fprintf(stdout, "ERROR %d: number of scales must be included between 1 and time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(scStep < 1 || (minScale+(numScales-1)*scStep) > N){
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

int HT::getNumScales(string str){
	int count = 0;
	int pos = 0;
	while((pos = str.find(scalesSep, pos)) != string::npos){
	    count++;
	    pos++;
	}
	count++;
	return count;
}

void HT::getScales(string str){
	int i = 0;
	int pos = 0;
	string token;
	while((pos = str.find(scalesSep)) != string::npos){
	    token = str.substr(0, pos);
	    scales[i] = stoi(token);
	    str.erase(0, pos+scalesSep.length());
	    i++;
	}
	scales[i] = stoi(str);
}

void HT::scalesWinFlucComp(){
	int L = getRangeLength(minScale, N);
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
	int L = getRangeLength(minScale, N);
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
