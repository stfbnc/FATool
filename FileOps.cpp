#include <iostream>

using namespace std;

FileOps::FileOps();

FileOps::~FileOps();

int FileOps::rows_number(char *file_name){
    FILE *f;
    int stop;
    int lines = 0;
    f = fopen(file_name,"r");
    while(!feof(f)){
        stop = fgetc(f);
        if(stop == '\n')
            lines++;
    }
    fclose(f);
    return lines;
}