#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "rhoDCCA.h"
#include "FileOps.h"
#include "DCCA.h"

using namespace std;

rhoDCCA::rhoDCCA(string fileName, string fileName2, int minWin, int maxWin, int polOrd)
		: file_name(fileName), file_name2(fileName2), min_win(minWin), max_win(maxWin), ord(polOrd) {}

rhoDCCA::~rhoDCCA() {}

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

void rhoDCCA::saveFile(string path_tot){
    FileOps fo = FileOps();
    FILE *f;
    f = fo.open_file(path_tot, "w");
    for(int i = 0; i < getRhoLength(); i++)
        fprintf(f, "%d %lf\n", i+min_win, rho[i]);
    fclose(f);
}

//void rhoDCCA::plot(){
//
//}
