#include "HTsingleScale.h"
#include "MFDFAsingleQ.h"

HTsingleScale::HTsingleScale(string file_name_, int scale_)
		: FA()
{
	file_name = file_name_;
	scale = scale_;
    checkFileExistence(file_name);
	N = setTsLength(file_name);
    checkInputs();
    allocateMemory();
}

HTsingleScale::~HTsingleScale(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<double>(F);
	delAlloc<double>(Ht);
}

void HTsingleScale::checkInputs(){
	//windows size
	if(scale < 3){
		fprintf(stdout, "ERROR %d: scale must be greater than 2\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}else if(scale > N){
		fprintf(stdout, "ERROR %d: scale must be smaller than time series length\n", WIN_SIZE_FAILURE);
		exit(WIN_SIZE_FAILURE);
	}
}

void HTsingleScale::allocateMemory(){
	t = new double [N];
	y = new double [N];
	F = new double [getRangeLength(scale, N)];
	Ht = new double [getRangeLength(scale, N)];
}

int HTsingleScale::getTsLength(){
	return N;
}

void HTsingleScale::setFlucVectors(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
    FileOps fo = FileOps();
	//time series vector
	double pn[N], pn_nomean[N];
    FILE *f;
    f = fo.open_file(file_name, "r");
    for(int i = 0; i < N; i++)
        fscanf(f, "%lf", &pn[i]);
    fclose(f);
	//time vector
    ao.double_range(t, N, 1.0);
    //time series minus its mean
    mo.subtract_mean(pn, N, pn_nomean);
    //cumulative sum
    mo.cumsum(pn_nomean, y, N);
}
    
void HTsingleScale::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(scale, N);
    double t_fit[scale], y_fit[scale], diff_vec[scale];
    //computation
    int start_lim, end_lim;
    double ang_coeff, intercept;
	ao.zero_vec(t_fit, scale);
	ao.zero_vec(y_fit, scale);
	ao.zero_vec(diff_vec, scale);
	ao.zero_vec(F, range);
	for(int v = 0; v <= N-scale; v++){
		start_lim = v;
		end_lim = v + scale - 1;
		ao.slice_vec(t, t_fit, start_lim, end_lim);
		ao.slice_vec(y, y_fit, start_lim, end_lim);
		mo.lin_fit(scale, t_fit, y_fit, &ang_coeff, &intercept);
		for(int j = 0; j < scale; j++)
			diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
		F[v] = sqrt(mo.mean(diff_vec, scale));
	}
}
// l'interfaccia puo' calcolare H facendo un fit in un untervallo qualsiasi, anche dopo aver fatto l'analisi
void HTsingleScale::H_loglogFit(int a, int b){
	int start = 4;
	int end = N / 5;
	MFDFAsingleQ dfaQ0 = MFDFAsingleQ(file_name, start, end, 1, 0.0);
	dfaQ0.setFlucVectors();
	dfaQ0.winFlucComp();
	dfaQ0.H_loglogFit(start, end);
	double Hq0 = dfaQ0.getH();
	double Hq0_intercept = dfaQ0.getH_intercept();
	
    MathOps mo = MathOps();
	int range = getRangeLength(scale, N);
	double Regfit, logscale;
	Regfit = Hq0_intercept + Hq0 * log(scale);
    logscale = log(range) - log(scale);
    for(int i = 0; i < range; i++){
        Ht[i] = (Regfit - log(F[i]))/(double) logscale + Hq0;
    }
}

string HTsingleScale::outFileStr(){
	return "/"+HTsS_FN_START+"_"+to_string(scale)+"_"+file_name.substr(file_name.find_last_of("/")+1);
}
// posso salvare il file di tutto il range per poi eventualmente ricaricarlo per rifare e salvare il grafico in un altro range
void HTsingleScale::saveFile(string path_tot){
    FileOps fo = FileOps();
	int range = getRangeLength(scale, N);
	FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
	fprintf(f, "#scale=%d\n", scale);
    for(int i = 0; i < range; i++)
        fprintf(f, "%lf\n", Ht[i]);
    fclose(f);
}
