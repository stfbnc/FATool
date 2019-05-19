#include <iostream>
#include "ArrayOps.h"

using namespace std;

ArrayOps::ArrayOps() {}

ArrayOps::~ArrayOps() {}

void ArrayOps::slice_vec(double *all_vec, double *sliced_vec, int start, int end){
    for(int i = 0; i <= (end - start); i++)
        sliced_vec[i] = all_vec[start+i];
}

void ArrayOps::concatenate(double *vec_to, double *vec_from1, double *vec_from2, int L){
    for(int i = 0; i < L; i++){
        vec_to[i] = vec_from1[i];
        vec_to[i+L] = vec_from2[i];
    }
}

void ArrayOps::vec_copy(double *vec_to, double *vec_from, int L){
    for(int i = 0; i < L; i++)
        vec_to[i] = vec_from[i];
}

void ArrayOps::zero_vec(double *vec, int L){
    for(int i = 0; i < L; i++)
        vec[i] = 0.0;
}

void ArrayOps::double_range(double *vec, int L, double start, double step){
	for(int i = 0; i < L; i++)
        vec[i] = static_cast<double>(i * step + start);
}

void ArrayOps::int_range(int *vec, int L, int start, int step){
	for(int i = 0; i < L; i++)
        vec[i] = i * step + start;
}
