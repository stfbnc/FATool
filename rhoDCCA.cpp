#include "rhoDCCA.h"

rhoDCCA::rhoDCCA(string file_name_, string file_name2_, int min_win_, int max_win_, int ord_){
	file_name = file_name_;
	file_name2 = file_name2_;
	min_win = min_win_;
	max_win = max_win_;
	ord = ord_;
	L = 0;
	rho = NULL;
}

rhoDCCA::~rhoDCCA() {
	delete [] rho;
}

void rhoDCCA::computeRho(){
    DCCA dccaXX = DCCA(file_name, file_name, min_win, max_win, ord);
    dccaXX.setFlucVectors();
    dccaXX.winFlucComp();
    double *Fxx = dccaXX.getF();
    DCCA dccaYY = DCCA(file_name2, file_name2, min_win, max_win, ord);
    dccaYY.setFlucVectors();
    dccaYY.winFlucComp();
    double *Fyy = dccaYY.getF();
    DCCA dccaXY = DCCA(file_name, file_name2, min_win, max_win, ord, CORR_DCCA);
    dccaXY.setFlucVectors();
    dccaXY.winFlucComp();
    double *Fxy = dccaXY.getF();
    L = dccaXY.getRangeLength(min_win, max_win);
    rho = new double [L];
    for(int i = 0; i < L; i++)
        rho[i] = Fxy[i] / (double)(Fxx[i] * Fyy[i]);
}

int rhoDCCA::getRhoLength(){
    return L;
}

string rhoDCCA::outFileStr(){
	return "/"+RHODCCA_FN_START+"_"+to_string(min_win)+"_"+to_string(min_win)+"_"
			+file_name.substr(file_name.find_last_of("/")+1)+"_"
			+file_name2.substr(file_name2.find_last_of("/")+1);
}

void rhoDCCA::saveFile(string path_tot){
    FileOps fo = FileOps();
    FILE *f;
    f = fo.open_file(path_tot+outFileStr(), "w");
    for(int i = 0; i < getRhoLength(); i++)
        fprintf(f, "%d %lf\n", i+min_win, rho[i]);
    fclose(f);
}

//void rhoDCCA::plot(){
//
//}
