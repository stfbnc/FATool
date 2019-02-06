#ifndef FA
#define FA

using namespace std;

class FA
{
public:
    FA(double *pn_, int min_win_, int max_win_ int ord_, int rev_seg_)
	  : min_win(min_win_), max_win(max_win_), ord(ord_), rev_seg(rev_seg_) {}
	virtual ~FA() {}
	virtual void SetConstVar(string, int, int, int) = 0;
	virtual void CreateVectors() = 0;
	virtual void WinFlucComp() = 0;
	virtual double H_loglogFit(double *, double *) = 0;
protected:
	int N;
	int min_win;
	int max_win;
	int ord;
	int rev_seg;
	double *t;
	double *y;
	int *s;
	int *F;
};

#endif
