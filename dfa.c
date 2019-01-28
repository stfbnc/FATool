#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//FUNCTIONS DECLARATION
int rows_number(char *);
double mean(double *, int);
void cumsum(double *, double *, int);
void slice_vec(double *, double *, int, int);
int lin_fit(int, const double *, const double *, double *, double *, double *);
void concatenate(double *, double *, double *, int);
void vec_copy(double *, double *, int);
void zero_vec(double *, int);

//MAIN
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
    //file length
    int N = rows_number(file_name);
    //time series
    double *pn;
    pn = malloc(N * sizeof(double));
    FILE *f;
    f = fopen(file_name, "r");
    for(int i = 0; i < N; i++){
        fscanf(f, "%lf", pn + i);
    }
    fclose(f);
    //time vector
    double *t;
    t = malloc(N * sizeof(double));
    for(int i = 0; i < N; i++){
        *(t + i) = (double)(i + 1);
    }
    //time series minus its mean
    double a_ave = mean(pn, N);
    double *pn_nomean;
    pn_nomean = malloc(N * sizeof(double));
    for(int i = 0; i < N; i++)
        *(pn_nomean + i) = *(pn + i) - a_ave;
    //cumulative sum
    double *y;
    y = malloc(N * sizeof(double));
    cumsum(pn_nomean, y, N);
    //defining parameters
    int max_win = 5;
    int end_dfa = N / max_win;
    int *s;
    int range_dfa = end_dfa - min_win + 1;
    s = malloc(range_dfa * sizeof(int));
    for(int i = 0; i < range_dfa; i++){
        *(s + i) = i + min_win;
    }
    //fluctuations vector and other arrays
    double *F, *F_nu1, *F_nu2, *t_fit, *y_fit, *diff_vec;
    int F_len = N / min_win;
    F = malloc(range_dfa * sizeof(double));
    F_nu1 = malloc(F_len * sizeof(double));
    F_nu2 = malloc(F_len * sizeof(double));
    t_fit = malloc(end_dfa * sizeof(double));
    y_fit = malloc(end_dfa * sizeof(double));
    diff_vec = malloc(end_dfa * sizeof(double));
    //computation
    int N_s;
    int start_lim, end_lim;
    double ang_coeff, intercept, r_coeff;
    for(int i = 0; i < range_dfa; i++){
        N_s = N / s[i];
        zero_vec(F_nu1, F_len);
        for(int v = 0; v < N_s; v++){
            zero_vec(t_fit, end_dfa);
            zero_vec(y_fit, end_dfa);
            zero_vec(diff_vec, end_dfa);
            start_lim = v * s[i];
            end_lim = (v + 1) * s[i];
            slice_vec(t, t_fit, start_lim, end_lim);
            slice_vec(y, y_fit, start_lim, end_lim);
            lin_fit(s[i], t_fit, y_fit, &ang_coeff, &intercept, &r_coeff);
            for(int j = 0; j < s[i]; j++)
                diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
            F_nu1[v] = mean(diff_vec, s[i]);
        }
        if(rev_seg == 1){
            zero_vec(F_nu2, F_len);
            for(int v = 0; v < N_s; v++){
                zero_vec(t_fit, end_dfa);
                zero_vec(y_fit, end_dfa);
                zero_vec(diff_vec, end_dfa);
                start_lim = v * s[i] + (N - N_s * s[i]);
                end_lim = (v + 1) * s[i] + (N - N_s * s[i]);
                slice_vec(t, t_fit, start_lim, end_lim);
                slice_vec(y, y_fit, start_lim, end_lim);
                lin_fit(s[i], t_fit, y_fit, &ang_coeff, &intercept, &r_coeff);
                for(int j = 0; j < s[i]; j++)
                    diff_vec[j] = pow((y_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
                F_nu2[v] = mean(diff_vec, s[i]);
            }
            F[i] = sqrt((mean(F_nu1, N_s) + mean(F_nu2, N_s)) / (double)2);
        }else{
            F[i] = sqrt(mean(F_nu1, N_s));
        }
    }
    free(F_nu1);
    free(F_nu2);
    free(t_fit);
    free(y_fit);
    free(diff_vec);
    //HURST EXPONENT
    double *log_s, *log_F, H, H_intercept, H_rcoeff;
    log_s = (double*)malloc(range_dfa * sizeof(double));
	if(!log_s)
        printf("no log_s\n");
    log_F = (double*)malloc(range_dfa * sizeof(double));
	if(!log_F)
        printf("no log_F\n");
    for(int i = 0; i < range_dfa; i++){
        *(log_s + i) = log(s[i]);
        *(log_F + i) = log(F[i]);
    }
    lin_fit(range_dfa, log_s, log_F, &H, &H_intercept, &H_rcoeff);
    f = fopen(path_tot, "w");
    for(int i = 0; i < range_dfa; i++)
        fprintf(f, "%d %lf %lf %lf\n", s[i], F[i], H_intercept + log_s[i] * H, H);
    fclose(f);
    free(F);
	free(log_s);
	free(log_F);
    return 0;
    
}

//FUNCTIONS
int rows_number(char *file_name){
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

double mean(double *vec, int L){
    double avg = 0.0;
    for(int i = 0; i < L; i++)
        avg += *(vec + i);
    avg /= (double)L;
    return avg;
}

void cumsum(double *vec, double *sum_vec, int L){
    *sum_vec = *vec;
    for(int i = 1; i < L; i++)
        *(sum_vec + i) = *(sum_vec + i - 1) + *(vec + i);
}

void slice_vec(double *all_vec, double *sliced_vec, int start, int end){
    for(int i = 0; i <= (end - start); i++)
        *(sliced_vec + i) = *(all_vec + start + i);
}

int lin_fit(int L, const double *x, const double *y, double *m, double *q, double *r){
    double sumx = 0.0;
    double sumx2 = 0.0;
    double sumxy = 0.0;
    double sumy = 0.0;
    double sumy2 = 0.0;
    for(int i = 0;i < L; i++){
        sumx += x[i];
        sumx2 += x[i] * x[i];
        sumxy += x[i] * y[i];
        sumy += y[i];
        sumy2 += y[i] * y[i];
    }
    double denom = (L * sumx2 - sumx * sumx);
    if(denom == 0){
        *m = 0;
        *q = 0;
        if(r)
            *r = 0;
        return 1;
    }
    *m = (L * sumxy - sumx * sumy) / denom;
    *q = (sumy * sumx2 - sumx * sumxy) / denom;
    if(r != NULL){
        *r = (sumxy - sumx * sumy / (double)L) /
              sqrt((sumx2 - (sumx * sumx) / L) *
              (sumy2 - (sumy * sumy) / L));
    }
    return 0;
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
