#ifndef DFA
#define DFA

class dfa
{
public:
    dfa(String, int, int, int);
	~dfa();
private:
	string file_name;
	int N;
	int min_win;
	int max_win;
	int ord;
	int rev_seg;
	int end_dfa;
	int dfa_range;
	double *t;
	double *y;
	int *s;
	int *F;
};

#endif
