#include "math_ops.h"

MathOps::MathOps() {}

MathOps::~MathOps() {}

double MathOps::mean(std::vector<double> vec, int L)
{
    double avg = 0.0;
    for(int i = 0; i < L; i++)
        avg += vec.at(i);
    avg /= static_cast<double>(L);
    return avg;
}

double MathOps::nanMean(std::vector<double> vec, int L)
{
    double avg = 0.0;
	int cnt = 0;
    for(int i = 0; i < L; i++){
        if(std::isnan(vec.at(i))){
			cnt++;
		}else{
            avg += vec.at(i);
		}
    }
    avg /= static_cast<double>(L - cnt);
    return avg;
}

double MathOps::customMean(std::vector<double> vec, int vecL, int L)
{
    double avg = 0.0;
    for(int i = 0; i < vecL; i++)
        avg += vec.at(i);
    avg /= static_cast<double>(L);
    return avg;
}

void MathOps::subtractMean(std::vector<double> vec, int L, std::vector<double> &vecNomean)
{
	double avg = mean(vec, L);
	for(int i = 0; i < L; i++)
        vecNomean.push_back(vec.at(i) - avg);
}

int MathOps::minVal(int a, int b)
{
    if(a <= b)
        return a;
    else
        return b;
}
	
void MathOps::cumsum(std::vector<double> vec, std::vector<double> &sumVec, int L)
{
    sumVec.push_back(vec.at(0));
    for(int i = 1; i < L; i++)
        sumVec.push_back(sumVec.at(i-1) + vec.at(i));
}

void MathOps::linFit(int L, const std::vector<double> x, const std::vector<double> y, double *m, double *q){
    double sumx = 0.0;
    double sumx2 = 0.0;
    double sumxy = 0.0;
    double sumy = 0.0;
    double sumy2 = 0.0;
    for(int i = 0; i < L; i++){
        sumx += x.at(i);
        sumx2 += x.at(i) * x.at(i);
        sumxy += x.at(i) * y.at(i);
        sumy += y.at(i);
        sumy2 += y.at(i) * y.at(i);
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

void MathOps::polyFit(int obs, int degree, std::vector<double> dx, std::vector<double> dy, std::vector<double> &store)
{
    gsl_multifit_linear_workspace *ws;
    gsl_matrix *cov, *X;
    gsl_vector *y, *c;
    double chisq;
    int i, j;

    X = gsl_matrix_alloc(obs, degree);
    y = gsl_vector_alloc(obs);
    c = gsl_vector_alloc(degree);
    cov = gsl_matrix_alloc(degree, degree);

    for(i = 0; i < obs; i++){
        for(j = 0; j < degree; j++){
            gsl_matrix_set(X, i, j, pow(dx.at(i), j));
        }
        gsl_vector_set(y, i, dy.at(i));
    }

    ws = gsl_multifit_linear_alloc(obs, degree);
    gsl_multifit_linear(X, y, c, cov, &chisq, ws);

    for(i = 0; i < degree; i++)
        store.push_back(gsl_vector_get(c, i));

    gsl_multifit_linear_free(ws);
    gsl_matrix_free(X);
    gsl_matrix_free(cov);
    gsl_vector_free(y);
    gsl_vector_free(c);
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

void MathOps::gaussRand(std::vector<double> &vec, int len, std::mt19937 generator){
    std::normal_distribution<> d{0, 1};
    for(int i = 0; i < len; i++)
        vec.push_back(d(generator));
}

int MathOps::quantile(int len, double q)
{
    int idx = round(q * len);
    if(idx == len)
        idx = len - 1;
    return idx;
}
