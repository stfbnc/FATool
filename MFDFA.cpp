#include "MFDFA.h"

MFDFA::MFDFA(string file_name_, int min_win_, int max_win_, int ord_, double qIn_, int Nq_, double stepq_, int rev_seg_)
	: MFDFAsingleQ(file_name_, min_win_, max_win_, ord_, qIn_, rev_seg_)
{
	Nq = Nq_;
	stepq = stepq_;
	qRange = NULL;
	flucMtx = NULL;
	Hq = NULL;
	H_interceptq = NULL;
	checkInputs();
    allocateQmemory();
}

MFDFA::~MFDFA(){
	delAlloc<double>(qRange);
	delAlloc<double>(Hq);
	delAlloc<double>(H_interceptq);
	del2Alloc<double>(flucMtx, getRangeLength(min_win, max_win));
}

void MFDFA::checkInputs(){
	//windows size
	if(Nq < 1){
		fprintf(stdout, "ERROR %d: number of qs must be greater than 1\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
}

void MFDFA::allocateQmemory(){
    qRange = new double [Nq];
    Hq = new double [Nq];
    H_interceptq = new double [Nq];
    flucMtx = new double* [getRangeLength(min_win, max_win)];
    for(int i = 0; i < getRangeLength(min_win, max_win); i++){
    	flucMtx[i] = new double [Nq];
    }
}

void MFDFA::setQrange(double start, int len, double step){
    ArrayOps ao = ArrayOps();
    ao.double_range(qRange, len, start, step);
}

void MFDFA::winFlucComp(){
	setQrange(q, Nq, stepq);
	int Lq = getRangeLength(min_win, max_win);
	for(int i = 0; i < Nq; i++){
		q = qRange[i];
		MFDFAsingleQ::winFlucComp();
		for(int j = 0; j < Lq; j++){
			flucMtx[j][i] = F[j];
		}
		H_loglogFit(min_win, max_win);
		Hq[i] = getH();
		H_interceptq[i] = getH_intercept();
	}
}

string MFDFA::outFileStr(){
	return "/"+MFDFA_FN_START+"_"+to_string(min_win)+"_"+to_string(max_win)+"_q"+to_string((int)qRange[0])+"_"+
			to_string((int)qRange[Nq-1])+"_"+file_name.substr(file_name.find_last_of("/")+1);
}

void MFDFA::saveFile(string path_tot){
	FileOps fo = FileOps();
	int Lq = getRangeLength(min_win, max_win);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
	fprintf(f, "#q ");
	for(int i = 0; i < Nq; i++){
		i == Nq-1 ? fprintf(f, "%lf\n", qRange[i]) : fprintf(f, "%lf ", qRange[i]);
	}
    for(int i = 0; i < Lq; i++){
		fprintf(f, "%d ", s[i]);
		for(int j = 0; j < Nq; j++){
			j == Nq-1 ? fprintf(f, "%lf\n", flucMtx[i][j]) : fprintf(f, "%lf ", flucMtx[i][j]);
		}
	}
    fclose(f);
}

string MFDFA::qoutFileStr(){
	return "/"+MFDFA_FN_START+"_q"+to_string((int)qRange[0])+"_"+to_string((int)qRange[Nq-1])+
			"_"+file_name.substr(file_name.find_last_of("/")+1);
}

void MFDFA::qsaveFile(string path_tot){
	FileOps fo = FileOps();
	FILE *f;
    f = fo.open_file(path_tot+qoutFileStr(), "w");
    for(int i = 0; i < Nq; i++){
		fprintf(f, "%lf %lf %lf\n", qRange[i], Hq[i], H_interceptq[i]);
	}
    fclose(f);
}

/*void MFDFA::plot(){
	
}*/

/*void MFDFA::qplot(){
	
}*/
