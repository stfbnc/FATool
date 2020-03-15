#ifndef ARRAY_OPS_H
#define ARRAY_OPS_H

#include <iostream>
#include <vector>
#include <cmath>

class ArrayOps
{
public:
    ArrayOps();
	~ArrayOps();
    void sliceVec(std::vector<double> allVec, std::vector<double> &slicedVec, int start, int end);
    void concatenate(std::vector<double> &vecTo, std::vector<double> vecFrom1, std::vector<double> vecFrom2, int L);
    void vecCopy(std::vector<double> &vecTo, std::vector<double> vecFrom, int L);
    void zeroVec(std::vector<double> &vec, int L);
    void doubleRange(std::vector<double> &vec, int L, double start, double step=1.0);
    void intRange(std::vector<int> &vec, int L, int start, int step=1);
    void extractColumn(std::vector<double> mtx, int l, int h, int col, std::vector<double> &vec);
    void noNan(std::vector<double> vec, int L, std::vector<double> &vecNoNan);
};

#endif
