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
    open_file(f, f_name, "r");
    while(!feof(f)){
        stop = fgetc(f);
        if(stop == '\n')
            lines++;
    }
    fclose(f);
    return lines;
}

void FileOps::open_file(FILE *f, string f_name, const char *mode){
    char file_name[f_name.length()+1];
    strcpy(file_name, f_name.c_str());
    f = fopen(file_name, mode);
}
