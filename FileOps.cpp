#include <iostream>
#include <cstdio>
#include <cstring>
#include "FileOps.h"

using namespace std;

FileOps::FileOps() {}

FileOps::~FileOps() {}

int FileOps::rows_number(string f_name){
    int stop;
    int lines = 0;
    FILE *f;
    f = open_file(f_name, "r");
    while(!feof(f)){
        stop = fgetc(f);
        if(stop == '\n')
            lines++;
    }
    fclose(f);
    return lines;
}

FILE* FileOps::open_file(string f_name, const char *mode){
    char file_name[f_name.length()+1];
    strcpy(file_name, f_name.c_str());
    FILE *fl;
    fl = fopen(file_name, mode);
    return fl;
}
