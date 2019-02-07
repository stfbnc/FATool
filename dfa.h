#ifndef DFA
#define DFA

class DFA : public FA
{
public:
    DFA(string, int, int, int, int);
	~DFA()
	int GetTsLength();
	int GetNumScales(int, int);
	void SetFlucVectors();
	void WinFlucComp();
	double H_loglogFit(int, int);
	void SaveFile(string);
};

#endif
