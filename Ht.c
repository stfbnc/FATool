#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//FUNCTIONS DECLARATION
int rows_number(char *);
//void split(char *, char, int, int *);
double mean(double *, int);
void cumsum(double *, double *, int);
void slice_vec(double *, double *, int, int);
int lin_fit(int, const double *, const double *, double *, double *, double *);
void zero_vec(double *, int);

//MAIN
int main(int argc, char **argv){
    
    int input_args = 5;
    if(argc < input_args){
        printf("Not enough input arguments!\n");
        return 99;
    }else if(argc > input_args){
        printf("Too many input arguments!\n");
        return 99;
    }
    char file_name[255];
    sprintf(file_name, "%s", argv[1]);
    //multiple scales are possible
    //scale is a string of ints separated by ","
    //////////////////////////////////
    //char scale[255];
    //sprintf(scale, "%s", argv[2]);
    int scale = atoi(argv[2]);
    /*int idx = 0;
    while(scale[idx] != '\0'){
        idx++;
    }
    int *win_len;
    win_len = malloc(idx * sizeof(int));
    printf("idx = %d\n", idx);
    split(scale, ',', idx, win_len);
    for(int i = 0; i < idx; i++){
        printf("%d\n", win_len[i]);
        printf("\n");
    }*/
    //////////////////////////////////
    int m = atoi(argv[3]);
    char path_tot[255];
    sprintf(path_tot, "%s", argv[4]);
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
    printf("file read <%d>\n", N);
    //time vector
    double *t;
    t = malloc(N * sizeof(double));
    for(int i = 0; i < N; i++)
        *(t + i) = (double)(i + 1);
    //time series minus its mean
    double a_ave = mean(pn, N);
    double *pn_nomean;
    pn_nomean = malloc(N * sizeof(double));
    for(int i = 0; i < N; i++)
        *(pn_nomean + i) = *(pn + i) - a_ave;
    //cumulative sum
    double *X;
    X = malloc(N * sizeof(double));
    cumsum(pn_nomean, X, N);
    printf("cumsum done\n");
    //defining parameters
    int scmin = 4;
    int max_win = 5;
    int scmax = N / max_win;
    int scres = 20;
    double step = (scmax - scmin) /(double) scres;
    int range0 = scres + 1;
    int *scale0;
    scale0 = malloc(range0 * sizeof(int));
    for(int i = 0; i < range0; i++)
        *(scale0 + i) = (int)(scmin + i * step);
    //Fluctuations at q = 0
    int num_seg, start_lim, end_lim;
    double ang_coeff, intercept, r_coeff;
    double *t_fit, *X_fit, *diff_vec, *RMS0, *Fq0;
    int flctLen = N / scmin;
    t_fit = malloc(scmax * sizeof(double));
    X_fit = malloc(scmax * sizeof(double));
    diff_vec = malloc(scmax * sizeof(double));
    RMS0 = malloc(flctLen * sizeof(double));
    Fq0 = malloc(range0 * sizeof(double));
    for(int i = 0; i < range0; i++){
        num_seg = N / scale0[i];
        for(int v = 0; v < num_seg; v++){
            zero_vec(t_fit, scmax);
            zero_vec(X_fit, scmax);
            zero_vec(diff_vec, scmax);
            start_lim = v * scale0[i];
            end_lim = (v + 1) * scale0[i];
            slice_vec(t, t_fit, start_lim, end_lim);
            slice_vec(X, X_fit, start_lim, end_lim);
            lin_fit(scale0[i], t_fit, X_fit, &ang_coeff, &intercept, &r_coeff);
            for(int j = 0; j < scale0[i]; j++)
                diff_vec[j] = pow((X_fit[j] - (intercept + ang_coeff * t_fit[j])), 2.0);
            RMS0[v] = log(mean(diff_vec, scale0[i]));
        }
        Fq0[i] = exp(0.5 * mean(RMS0, num_seg));
    }
    printf("Fluct0 done\n");
    
    //da modificare per più scale (le malloc sono sui vettori con max(scale) e vengono azzerati sempre)
    int HtLen = N - scale + 1;
    double *X_fitH, *diff_vecH, *RMS, *t_fitH;
    X_fitH = malloc(scale * sizeof(double));
    t_fitH = malloc(scale * sizeof(double));
    diff_vecH = malloc(scale * sizeof(double));
    RMS = malloc(HtLen * sizeof(double));
    zero_vec(X_fitH, scale);
    zero_vec(t_fitH, scale);
    zero_vec(diff_vecH, scale);
    zero_vec(RMS, HtLen);
    // modificare con un loop sulle scale
    for(int v = 0; v <= N - scale; v++){
        start_lim = v;
        end_lim = v + scale - 1;
        for(int j = 0; j < scale; j++)
            *(t_fitH + j) = (double)(start_lim + j);
        slice_vec(X, X_fitH, start_lim, end_lim);
        lin_fit(scale, t_fitH, X_fitH, &ang_coeff, &intercept, &r_coeff);
        for(int j = 0; j < scale; j++)
            diff_vec[j] = pow((X_fitH[j] - (intercept + ang_coeff * t_fitH[j])), 2.0);
        RMS[v] = sqrt(mean(diff_vec, scale));
    }
    printf("FluctT done\n");
    
    //LOCAL HURST
    double *log_scale0, *log_Fq0, Hq0, Hq0_intercept, Hq0_rcoeff;
    log_scale0 = malloc(range0 * sizeof(double));
    log_Fq0 = malloc(range0 * sizeof(double));
    for(int i = 0; i < range0; i++){
        *(log_scale0 + i) = log(scale0[i]);
        *(log_Fq0 + i) = log(Fq0[i]);
    }
    lin_fit(range0, log_scale0, log_Fq0, &Hq0, &Hq0_intercept, &Hq0_rcoeff);
    //modificare in caso di più scale
    double Regfit, logscale;
    double *resRMS, *Ht;
    Regfit = Hq0_intercept + Hq0 * log(scale);
    logscale = log(HtLen) - log(scale);
    resRMS = malloc(HtLen * sizeof(double));
    Ht = malloc(HtLen * sizeof(double));
    for(int i = 0; i < HtLen; i++){
        *(resRMS + i) = Regfit - log(*(RMS + i));
        *(Ht + i) = *(resRMS + i) /(double) logscale + Hq0;
    }
    printf("Ht done\n");
    //Local Hurst file
    char path_file[255];
    sprintf(path_file, "%s/Ht_%d.txt", path_tot, scale); //modificare in caso di più scale
    f = fopen(path_file, "w");
    for(int i = 0; i < HtLen; i++)
        fprintf(f, "%lf\n", Ht[i]);
    fclose(f);
    printf("Output file done\n");
    
    free(pn); free(t); free(pn_nomean); free(X); free(scale0);
    free(t_fit); free(X_fit); free(diff_vec); free(RMS0); free(Fq0);
    free(X_fitH); free(t_fitH); free(diff_vecH); free(RMS);
    free(log_scale0); free(log_Fq0);
    free(resRMS); free(Ht);
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

/*void split(char *to_split, char sep, int L, int *splitted){

}*/

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

void zero_vec(double *vec, int L){
    for(int i = 0; i < L; i++)
        *(vec + i) = 0.0;
}
