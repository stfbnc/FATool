#include <iostream>
#include <cmath>
#include "MathOps.h"

using namespace std;

MathOps::MathOps() {}

MathOps::~MathOps() {}

double MathOps::mean(double *vec, int L){
    double avg = 0.0;
    for(int i = 0; i < L; i++)
        avg += vec[i];
    avg /= (double)L;
    return avg;
}

double MathOps::nan_mean(double *vec, int L){
    double avg = 0.0;
	int cnt = 0;
    for(int i = 0; i < L; i++){
		if(isnan(vec[i])){
			cnt++;
		}else{
			avg += vec[i];
		}
	}	
    avg /= (double)(L - cnt);
    return avg;
}

double MathOps::custom_mean(double *vec, int vecL, int L){
    double avg = 0.0;
    for(int i = 0; i < vecL; i++)
        avg += vec[i];
    avg /= (double)L;
    return avg;
}

void MathOps::subtract_mean(double *vec, int L, double *vec_nomean){
	double avg = mean(vec, L);
	for(int i = 0; i < L; i++)
		vec_nomean[i] = vec[i] - avg;
}
	
void MathOps::cumsum(double *vec, double *sum_vec, int L){
    sum_vec[0] = vec[0];
    for(int i = 1; i < L; i++)
        sum_vec[i] = sum_vec[i-1] + vec[i];
}

void MathOps::nan_cumsum(double *vec, double *sum_vec, int L){
    sum_vec[0] = vec[0];
    for(int i = 1; i < L; i++){
		if(isnan(vec[i])){
			sum_vec[i] = sum_vec[i-1];
		}else{
			sum_vec[i] = sum_vec[i-1] + vec[i];
		}
	}
}

void MathOps::lin_fit(int L, const double *x, const double *y, double *m, double *q){
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
    if(denom == 0){
        *m = 0;
        *q = 0;
    }
    *m = (L * sumxy - sumx * sumy) / denom;
    *q = (sumy * sumx2 - sumx * sumxy) / denom;
}
