#ifndef MATH_OPS
#define MATH_OPS

class MathOps
{
public:
    MathOps();
	~MathOps();
	double mean(double *, int);
	double nan_mean(double *, int);
	void subtract_mean(double *, int, double *);
	void cumsum(double *, double *, int);
	void nan_cumsum(double *, double *, int);
	void lin_fit(int, const double *, const double *, double *, double *);
};

#endif
