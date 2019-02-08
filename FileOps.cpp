#include <iostream>
#include <cstdio>
#include <cstring>
#include "FileOps.h"

using namespace std;

FileOps::FileOps() {}

FileOps::~FileOps() {}

int FileOps::rows_number(string f_name){
	char file_name[f_name.length()+1];
	strcpy(file_name, f_name.c_str());
    FILE *f;
    int stop;
    int lines = 0;
    f = fopen(file_name, "r");
    while(!feof(f)){
        stop = fgetc(f);
        if(stop == '\n')
            lines++;
    }
    fclose(f);
    return lines;
}
