#include "math_ops.h"

MathOps::MathOps() {}

MathOps::~MathOps() {}

double MathOps::mean(double *vec, int L){
    double avg = 0.0;
    for(int i = 0; i < L; i++)
        avg += vec[i];
    avg /= static_cast<double>(L);
    return avg;
}

double MathOps::nanMean(double *vec, int L){
    double avg = 0.0;
	int cnt = 0;
    for(int i = 0; i < L; i++){
		if(std::isnan(vec[i])){
			cnt++;
		}else{
			avg += vec[i];
		}
    }
    avg /= static_cast<double>(L - cnt);
    return avg;
}

double MathOps::customMean(double *vec, int vecL, int L){
    double avg = 0.0;
    for(int i = 0; i < vecL; i++)
        avg += vec[i];
    avg /= static_cast<double>(L);
    return avg;
}

void MathOps::subtractMean(double *vec, int L, double *vecNomean){
	double avg = mean(vec, L);
	for(int i = 0; i < L; i++)
		vecNomean[i] = vec[i] - avg;
}

int MathOps::vecMin(int *vec, int len){
    int m = vec[0];
    for(int i = 1; i < len; i++)
        if(vec[i] < m)
            m = vec[i];
    return m;
}

int MathOps::minVal(int a, int b){
    if(a <= b)
        return a;
    else
        return b;
}
	
void MathOps::cumsum(double *vec, double *sumVec, int L){
    sumVec[0] = vec[0];
    for(int i = 1; i < L; i++)
        sumVec[i] = sumVec[i-1] + vec[i];
}

void MathOps::nanCumsum(double *vec, double *sumVec, int L){
    sumVec[0] = vec[0];
    for(int i = 1; i < L; i++){
		if(std::isnan(vec[i])){
			sumVec[i] = sumVec[i-1];
		}else{
			sumVec[i] = sumVec[i-1] + vec[i];
		}
	}
}

void MathOps::linFit(int L, const double *x, const double *y, double *m, double *q){
    double sumx = 0.0;
    double sumx2 = 0.0;
    double sumxy = 0.0;
    double sumy = 0.0;
    double sumy2 = 0.0;
    for(int i = 0; i < L; i++){
        sumx += x[i];
        sumx2 += x[i] * x[i];
        sumxy += x[i] * y[i];
        sumy += y[i];
        sumy2 += y[i] * y[i];
    }
    double denom = (L * sumx2 - sumx * sumx);
    if(denom == 0.0){
        *m = 0.0;
        *q = 0.0;
    }else{
        *m = (L * sumxy - sumx * sumy) / denom;
        *q = (sumy * sumx2 - sumx * sumxy) / denom;
    }
}

long long int MathOps::factorial(int a){
    if(a == 0 || a == 1)
        return 1;
    else
        return a * factorial(a-1);
}

long long int MathOps::binCoeff(int a, int b){
    long long int fac1 = factorial(a);
    long long int fac2 = factorial(b);
    long long int fac3 = factorial(a-b);
    return fac1 / (fac2 * fac3);
}