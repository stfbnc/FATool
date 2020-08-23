#ifndef MATH_OPS_H
#define MATH_OPS_H

#include <iostream>
#include <cmath>
#include <gsl/gsl_multifit.h>
#include <random>
#include <algorithm>

class MathOps
{
public:
    explicit MathOps();
    virtual ~MathOps();
    double mean(std::vector<double> vec, int L);
    double nanMean(std::vector<double> vec, int L);
    double customMean(std::vector<double> vec, int vecL, int L);
    void subtractMean(std::vector<double> vec, int L, std::vector<double> &vecNomean);
    int minVal(int a, int b);
    void cumsum(std::vector<double> vec, std::vector<double> &sumVec, int L);
    void nanCumsum(std::vector<double> vec, std::vector<double> &sumVec, int L);
    void linFit(int L, const std::vector<double> x, const std::vector<double> y, double *m, double *q);
    void polyFit(int obs, int degree, std::vector<double> dx, std::vector<double> dy, std::vector<double> &store);
    long long int factorial(int a);
    long long int binCoeff(int a, int b);
    void gaussRand(std::vector<double> &vec, int len, std::mt19937 generator);
    int quantile(int len, double q);

    template <class T>
    T vecMin(std::vector<T> vec, int len){
        T m = vec.at(0);
        for(int i = 1; i < len; i++)
            if(vec.at(i) < m)
                m = vec.at(i);
        return m;
    }

    template <class T>
    T vecMax(std::vector<T> vec, int len){
        T M = vec.at(0);
        for(int i = 1; i < len; i++)
            if(vec.at(i) > M)
                M = vec.at(i);
        return M;
    }
};

#endif
