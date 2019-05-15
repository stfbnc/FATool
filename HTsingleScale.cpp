#include "HTsingleScale.h"
#include "MFDFAsingleQ.h"

HTsingleScale::HTsingleScale(string file_name_, int scale_)
	: FA()
{
	file_name = file_name_;
	scale = scale_;
    checkFileExistence(file_name);
	N = setTsLength(file_name);
    allocateMemory();
}

HTsingleScale::~HTsingleScale(){
	delAlloc<double>(t);
	delAlloc<double>(y);
	delAlloc<double>(F);
	delAlloc<double>(Ht);
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
    
void HTsingleScale::winFlucComp(){
    MathOps mo = MathOps();
    ArrayOps ao = ArrayOps();
	int range = getRangeLength(scale, N);
    double *t_fit, *y_fit, *diff_vec;
    t_fit = new double [scale];
    y_fit = new double [scale];
    diff_vec = new double [scale];
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
    a = 0;
    b = 0;
}

void HTsingleScale::Ht_fit()
{
    int start = 4;
    int end = N / 5;
    int step = N / 20;
    if(step == 0)
        step = 1;
    MFDFAsingleQ dfaQ0 = MFDFAsingleQ(file_name, start, end, 1, 0.0, step);
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
        Ht[i] = (Regfit - log(F[i])) / static_cast<double>(logscale) + Hq0;
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

void HTsingleScale::plot(QCustomPlot *)
{

}
