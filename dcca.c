#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//FUNCTIONS DECLARATION
void dcca_comp(double *, double *, double *, int, int, int, int, double *, int);
int rows_number(char *);
double mean(double *, int, int);
void cumsum(double *, double *, int);
void slice_vec(double *, double *, int, int);
int lin_fit(int, const double *, const double *, double *, double *, double *);
void zero_vec(double *, int);

//MAIN
int main(int argc, char **argv){
    
    //inputs
    int input_args = 7;
    if(argc < input_args){
        printf("Not enough input arguments!\n");
        return 99;
    }else if(argc > input_args){
        printf("Too many input arguments!\n");
        return 99;
    }
    char file_name1[255]; //path+name for the first file
    sprintf(file_name1, "%s", argv[1]);
    char file_name2[255]; //path+name for the second file
    sprintf(file_name2, "%s", argv[2]);
    int smallest_win_size = atoi(argv[3]); //minimum size for the sliding window
    int biggest_win_num = atoi(argv[4]); //number of windows in the last step
    int pol_ord = atoi(argv[5]); //polynomial order for fit
    char path_out[255]; //path+name for the output file
    sprintf(path_out, "%s", argv[6]);
    
    //files length
    int N1 = rows_number(file_name1);
    int N2 = rows_number(file_name2);
    
    //time series
    double *ts1, *ts2;
    ts1 = malloc(N1 * sizeof(double));
    ts2 = malloc(N2 * sizeof(double));
    FILE *f1, *f2;
    f1 = fopen(file_name1, "r");
    for(int i = 0; i < N1; i++)
        fscanf(f1, "%lf", ts1 + i);
    fclose(f1);
    f2 = fopen(file_name2, "r");
    for(int i = 0; i < N2; i++)
        fscanf(f2, "%lf", ts2 + i);
    fclose(f2);
    
    //time series must have the same length
    int L = N1;
    if(N1 < N2){
        slice_vec(ts2, ts2, 0, N1-1);
    }else if(N1 > N2){
        slice_vec(ts1, ts1, 0, N2-1);
        L = N2;
    }
    
    //time vector
    double *time;
    time = malloc(L * sizeof(double));
    for(int i = 0; i < L; i++)
        *(time + i) = (double)(i + 1);

    //time series minus its mean
    double ts1_ave = mean(ts1, L, L);
    double ts2_ave = mean(ts2, L, L);
    double *ts1_nomean, *ts2_nomean;
    ts1_nomean = malloc(L * sizeof(double));
    ts2_nomean = malloc(L * sizeof(double));
    for(int i = 0; i < L; i++){
        *(ts1_nomean + i) = *(ts1 + i) - ts1_ave;
        *(ts2_nomean + i) = *(ts2 + i) - ts2_ave;
    }
    
    //cumulative sum
    double *ts1_cum, *ts2_cum;
    ts1_cum = malloc(L * sizeof(double));
    ts2_cum = malloc(L * sizeof(double));
    cumsum(ts1_nomean, ts1_cum, L);
    cumsum(ts2_nomean, ts2_cum, L);
    
    //dcca computation
    int biggest_win_size = L / biggest_win_num;
    int win_range = biggest_win_size - smallest_win_size + 1;
    double *F_DCCA, *F_DFA_x, *F_DFA_y, *rho;
    F_DCCA = malloc(win_range * sizeof(double));
    F_DFA_x = malloc(win_range * sizeof(double));
    F_DFA_y = malloc(win_range * sizeof(double));
    rho = malloc(win_range * sizeof(double));
    dcca_comp(time, ts1_cum, ts2_cum, L, smallest_win_size, biggest_win_size, pol_ord, F_DCCA, win_range);
    dcca_comp(time, ts1_cum, ts1_cum, L, smallest_win_size, biggest_win_size, pol_ord, F_DFA_x, win_range);
    dcca_comp(time, ts2_cum, ts2_cum, L, smallest_win_size, biggest_win_size, pol_ord, F_DFA_y, win_range);
    FILE *fOut;
    fOut = fopen(path_out, "w");
    for(int i = 0; i < win_range; i++){
        *(F_DFA_x + i) = sqrt(*(F_DFA_x + i));
        *(F_DFA_y + i) = sqrt(*(F_DFA_y + i));
        *(rho + i) = *(F_DCCA + i) /(double)(*(F_DFA_x + i) * *(F_DFA_y + i));
        fprintf(fOut, "%lf %lf %lf %lf\n", *(F_DCCA + i), *(F_DFA_x + i), *(F_DFA_y + i), *(rho + i));
    }
    fclose(fOut);
    
    free(ts1); free(ts2); free(time);
    free(ts1_nomean); free(ts2_nomean);
    free(ts1_cum); free(ts2_cum);
    free(F_DCCA); free(F_DFA_x); free(F_DFA_y); free(rho);

    return 0;
    
}

//FUNCTIONS
void dcca_comp(double *t, double *y1, double *y2, int N, int min_win_size, int last_win_len, int ord, double *F, int range_dcca){
    
    //fluctuations vector and other arrays
    int F_len = N - min_win_size;
    double *F_nu, *t_fit, *y_fit1, *y_fit2, *diff_vec;
    F_nu = malloc(F_len * sizeof(double));
    t_fit = malloc((last_win_len+1) * sizeof(double));
    y_fit1 = malloc((last_win_len+1) * sizeof(double));
    y_fit2 = malloc((last_win_len+1) * sizeof(double));
    diff_vec = malloc((last_win_len+1) * sizeof(double));
    //computation
    int s, N_s, start_lim, end_lim;
    double ang_coeff1, intercept1, r_coeff1, ang_coeff2, intercept2, r_coeff2;
    for(int i = 0; i < range_dcca; i++){
        s = i + min_win_size;
        N_s = N - s;
        zero_vec(F_nu, F_len);
        for(int v = 0; v < N_s; v++){
            zero_vec(t_fit, last_win_len+1);
            zero_vec(y_fit1, last_win_len+1);
            zero_vec(y_fit2, last_win_len+1);
            zero_vec(diff_vec, last_win_len+1);
            start_lim = v;
            end_lim = v + s;
            slice_vec(t, t_fit, start_lim, end_lim);
            slice_vec(y1, y_fit1, start_lim, end_lim);
            slice_vec(y2, y_fit2, start_lim, end_lim);
            lin_fit(s+1, t_fit, y_fit1, &ang_coeff1, &intercept1, &r_coeff1);
            lin_fit(s+1, t_fit, y_fit2, &ang_coeff2, &intercept2, &r_coeff2);
            for(int j = 0; j <= s; j++)
                *(diff_vec + j) = (*(y_fit1 + j) - (intercept1 + ang_coeff1 * *(t_fit + j))) * (*(y_fit2 + j) - (intercept2 + ang_coeff2 * *(t_fit + j)));
            *(F_nu + v) = mean(diff_vec, s+1, s-1);
        }
        *(F + i) = mean(F_nu, N_s, N_s);
    }
    free(F_nu); free(t_fit); free(y_fit1); free(y_fit2); free(diff_vec);
}

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

double mean(double *vec, int vecL, int L){
    double avg = 0.0;
    for(int i = 0; i < vecL; i++)
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
    for(int i = 0; i < L; i++){
        sumx += *(x + i);
        sumx2 += *(x + i) * *(x + i);
        sumxy += *(x + i) * *(y + i);
        sumy += *(y + i);
        sumy2 += *(y + i) * *(y + i);
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
