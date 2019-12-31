#ifndef ARRAY_OPS_H
#define ARRAY_OPS_H

class ArrayOps
{
public:
    ArrayOps();
	~ArrayOps();
	void sliceVec(double *allVec, double *slicedVec, int start, int end);
	void concatenate(double *vecTo, double *vecFrom1, double *vecFrom2, int L);
    void vecCopy(double *vecTo, double *vecFrom, int L);
    void zeroVec(double *vec, int L);
	void doubleRange(double *vec, int L, double start, double step=1.0);
	void intRange(int *vec, int L, int start, int step=1);
};

#endif
