#ifndef FAGLOBS_H
#define FAGLOBS_H
//libraries
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <limits>
#include <sys/stat.h>
//basic operations files
#include "ArrayOps.h"
#include "MathOps.h"
#include "FileOps.h"
//defines for parameters errors
#define RANGE_FAILURE -99
#define WIN_SIZE_FAILURE -98
#define POL_FAILURE -97
#define REV_SEG_FAILURE -96
#define FILE_FAILURE -95
//defines for dcca fluctuation vector
#define DEFAULT_DCCA "abs"
#define CORR_DCCA "sign"
//namespaces
using namespace std;
//strings
const string STR_SEP = ",";
const string HT_FN_START = "HT";
const string DCCA_FN_START = "DCCA";
const string DFA_FN_START = "DFA";
const string HTsS_FN_START = "HTsingleScale";
const string MFDFA_FN_START = "MFDFA";
const string MFDFAsQ_FN_START = "MFDFAsingleQ";
const string RHODCCA_FN_START = "rhoDCCA";

#endif
