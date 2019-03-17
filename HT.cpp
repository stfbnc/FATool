#include "HT.h"

HT::HT(string file_name_, int scale_, int Nscales_, int stepScale_)
	: HTsingleScale(file_name_, scale_)
{
	Nscales = Nscales_;
	minScale = scale_;
	stepScale = stepScale_;
	scales = NULL;
	HTmtx = NULL;
    checkInputs();
    allocateScalesMemory();
    ArrayOps ao = ArrayOps();
    ao.int_range(scales, Nscales, minScale, stepScale);
}

HT::HT(string fileName, string strScales)
		: HTsingleScale(fileName, stoi(strScales.substr(0, strScales.find_first_of(STR_SEP))))
{
	Nscales = getNumScales(strScales);
	scales = NULL;
	HTmtx = NULL;
	allocateScalesMemory();
	getScales(strScales);
	MathOps mo = MathOps();
	minScale = mo.vec_min(scales, Nscales);
}

HT::~HT(){
	delAlloc<int>(scales);
	del2Alloc<double>(HTmtx, getRangeLength(minScale, N));
}

void HT::checkInputs(){
	//windows size
	if(Nscales < 1 || Nscales > N){
		fprintf(stdout, "ERROR %d: number of scales must be included between 1 and time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(stepScale < 1 || (minScale+(Nscales-1)*stepScale) > N){
		fprintf(stdout, "ERROR %d: step must be strictly positive and such that scales are smaller than the time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
}

void HT::allocateScalesMemory(){
	scales = new int [Nscales];
	HTmtx = new double* [getRangeLength(minScale, N)];
	for(int i = 0; i < getRangeLength(minScale, N); i++){
		HTmtx[i] = new double [Nscales];
	}
}

int HT::getNumScales(string str){
	int count = 0;
	int pos = 0;
	while((pos = str.find(STR_SEP, pos)) != string::npos){
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
	while((pos = str.find(STR_SEP)) != string::npos){
	    token = str.substr(0, pos);
	    scales[i] = stoi(token);
	    str.erase(0, pos+STR_SEP.length());
	    i++;
	}
	scales[i] = stoi(str);
}

//void HT::scalesWinFlucComp(){
void HT::winFlucComp(){
	int L = getRangeLength(minScale, N);
	ArrayOps ao = ArrayOps();
	for(int i = 0; i < Nscales; i++){
		scale = scales[i];
		int Lscale = getRangeLength(scale, N);
		ao.zero_vec(Ht, L);
		HTsingleScale::winFlucComp();
		H_loglogFit(1, 1);
		for(int j = 0; j < Lscale; j++){
			HTmtx[j][i] = Ht[j];
		}
		for(int j = Lscale; j < L; j++){
			HTmtx[j][i] = numeric_limits<double>::quiet_NaN();
		}
	}
}

string HT::outFileStr(){
	return "/"+HT_FN_START+"_"+file_name.substr(file_name.find_last_of("/")+1);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void HT::saveFile(string path_tot){
	FileOps fo = FileOps();
	int L = getRangeLength(minScale, N);
	FILE *f;
	f = fo.open_file(path_tot+outFileStr(), "w");
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
