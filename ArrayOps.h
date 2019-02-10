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
	void double_range(double *, int, double, double=1.0);
	void int_range(int *, int, int, int=1);
};

#endif
