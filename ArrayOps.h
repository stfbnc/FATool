#ifndef ARRAY_OPS
#define ARRAY_OPS

class ArrayOps
{
public:
    ArrayOps();
	void slice_vec(double *, double *, int, int);
	void concatenate(double *, double *, double *, int);
    void vec_copy(double *, double *, int);
    void zero_vec(double *, int);
};

#endif
