#include "array_ops.h"

ArrayOps::ArrayOps() {}

ArrayOps::~ArrayOps() {}

void ArrayOps::sliceVec(double *allVec, double *slicedVec, int start, int end){
    for(int i = 0; i <= (end - start); i++)
        slicedVec[i] = allVec[start+i];
}

void ArrayOps::concatenate(double *vecTo, double *vecFrom1, double *vecFrom2, int L){
    for(int i = 0; i < L; i++){
        vecTo[i] = vecFrom1[i];
        vecTo[i+L] = vecFrom2[i];
    }
}

void ArrayOps::vecCopy(double *vecTo, double *vecFrom, int L){
    for(int i = 0; i < L; i++)
        vecTo[i] = vecFrom[i];
}

void ArrayOps::zeroVec(double *vec, int L){
    for(int i = 0; i < L; i++)
        vec[i] = 0.0;
}

void ArrayOps::doubleRange(double *vec, int L, double start, double step){
	for(int i = 0; i < L; i++)
        vec[i] = static_cast<double>(i * step + start);
}

void ArrayOps::intRange(int *vec, int L, int start, int step){
	for(int i = 0; i < L; i++)
        vec[i] = i * step + start;
}

void ArrayOps::extractColumn(double *mtx, int l, int h, int col, double *vec){
    for(int i = 0; i < h; i++)
        vec[i] = mtx[col+i*l];
}
