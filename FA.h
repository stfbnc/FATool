#ifndef FA
#define FA

using namespace std;

class FA
{
public:
    FA(string file_name_, int min_win_, int max_win_ int ord_, int rev_seg_)
	  : file_name(file_name_), min_win(min_win_), max_win(max_win_), ord(ord_), rev_seg(rev_seg_) {}
	virtual ~FA() {}
	virtual int GetTsLength() = 0;
	virtual int GetNumScales(int, int) = 0;
	virtual void SetFlucVectors() = 0;
	virtual void WinFlucComp() = 0;
	virtual double H_loglogFit(int, int) = 0;
	virtual void SaveFile(string) = 0;
protected:
	string file_name;
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
