#include "array_ops.h"

ArrayOps::ArrayOps() {}

ArrayOps::~ArrayOps() {}

void ArrayOps::sliceVec(std::vector<double> allVec, std::vector<double> &slicedVec, int start, int end)
{
    for(int i = 0; i <= (end - start); i++)
        slicedVec.push_back(allVec.at(start + i));
}

void ArrayOps::zeroVec(std::vector<double> &vec, int L)
{
    for(int i = 0; i < L; i++)
        vec.push_back(0.0);
}

void ArrayOps::doubleRange(std::vector<double> &vec, int L, double start, double step)
{
	for(int i = 0; i < L; i++)
        vec.push_back(static_cast<double>(i * step + start));
}

void ArrayOps::intRange(std::vector<int> &vec, int L, int start, int step)
{
	for(int i = 0; i < L; i++)
        vec.push_back(i * step + start);
}

void ArrayOps::extractColumn(std::vector<double> mtx, int l, int h, int col, std::vector<double> &vec)
{
    for(int i = 0; i < h; i++)
        vec.push_back(mtx.at(col + i * l));
}

void ArrayOps::noNan(std::vector<double> vec, int L, std::vector<double> &vecNoNan)
{
    int idx = 0;
    for(int i = 0; i < L; i++)
    {
        if(!std::isnan(vec.at(i)))
        {
            vecNoNan.push_back(vec.at(i));
            idx++;
        }
    }
}
