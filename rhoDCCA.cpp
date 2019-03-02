#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "rhoDCCA.h"
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"

using namespace std;

rhoDCCA::rhoDCCA(string fileName, string fileName2, int minWin, int maxWin, int polOrd)
		: file_name(fileName), file_name2(fileName2), min_win(minWin), max_win(maxWin), ord(polOrd) {}

rhoDCCA::~rhoDCCA() {}


