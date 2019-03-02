#ifndef RHO_DCCA_H
#define RHO_DCCA_H

#include "DCCA.h"

class rhoDCCA
{
public:
    rhoDCCA(string, string, int, int, int);
	~rhoDCCA();
private:
	string file_name;
	string file_name2;
	int min_win;
	int max_win;
	int ord;
};

#endif
