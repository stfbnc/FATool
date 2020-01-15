#ifndef MATH_OPS_H
#define MATH_OPS_H

#include <iostream>
#include <cmath>
#include <gsl/gsl_multifit.h>
#include <random>

class MathOps
{
public:
    MathOps();
	~MathOps();
	double mean(double *vec, int L);
	double nanMean(double *vec, int L);
    double customMean(double *vec, int vecL, int L);
	void subtractMean(double *vec, int L, double *vecNomean);
    //int vecMin(int *vec, int len);
    //int vecMax(int *vec, int len);
    int minVal(int a, int b);
	void cumsum(double *vec, double *sumVec, int L);
	void nanCumsum(double *vec, double *sumVec, int L);
    void linFit(int L, const double *x, const double *y, double *m, double *q);
    void polyFit(int obs, int degree, double *dx, double *dy, double *store);
    long long int factorial(int a);
    long long int binCoeff(int a, int b);
    void gaussRand(double *vec, int len);

    template <class T>
    T vecMin(T *vec, int len){
        T m = vec[0];
        for(int i = 1; i < len; i++)
            if(vec[i] < m)
                m = vec[i];
        return m;
    }

    template <class T>
    T vecMax(T *vec, int len){
        T M = vec[0];
        for(int i = 1; i < len; i++)
            if(vec[i] > M)
                M = vec[i];
        return M;
    }
};

#endif
