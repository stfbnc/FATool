#ifndef ARRAY_OPS_H
#define ARRAY_OPS_H

class ArrayOps
{
public:
    ArrayOps();
	~ArrayOps();
	void slice_vec(double *, double *, int, int);
	void concatenate(double *, double *, double *, int);
    void vec_copy(double *, double *, int);
    void zero_vec(double *, int);
	void double_range(double *, int, double, double);
	void int_range(double *, int, int, int);
};

#endif
