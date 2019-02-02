#include <iosream>

public class ArrayOps{
	
	
	
}

int main(int argc, char **argv){
    
    int input_args = 6;
    if(argc < input_args){
        printf("Not enough input arguments!\n");
        return 99;
    }else if(argc > input_args){
        printf("Too many input arguments!\n");
        return 99;
    }
    char file_name[255];
    sprintf(file_name, "%s", argv[1]);
    int min_win = atoi(argv[2]);
    int ord = atoi(argv[3]);
    int rev_seg = atoi(argv[4]);
    char path_tot[255];
    sprintf(path_tot, "%s/dfa.txt", argv[5]);



    
}

//FUNCTIONS

void slice_vec(double *all_vec, double *sliced_vec, int start, int end){
    for(int i = 0; i <= (end - start); i++)
        *(sliced_vec + i) = *(all_vec + start + i);
}

void concatenate(double *vec_to, double *vec_from1, double *vec_from2, int L){
    for(int i = 0; i < L; i++){
        vec_to[i] = vec_from1[i];
        vec_to[i + L] = vec_from2[i];
    }
}

void vec_copy(double *vec_to, double *vec_from, int L){
    for(int i = 0; i < L; i++)
        *(vec_to + i) = *(vec_from + i);
}

void zero_vec(double *vec, int L){
    for(int i = 0; i < L; i++)
        *(vec + i) = 0.0;
}
