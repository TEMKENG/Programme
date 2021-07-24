#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "histogramm.h"


Histogram *getHistogram(FILE *file);

int *split(char *line, ssize_t length);

int *max(const int *list, size_t length);

void printList(const int *list, ssize_t length){
    printf("\n");
    for (int i = 0; i < length; i++){
        printf("%i ", list[i]);
    }

}
int main() {
    clock_t t = clock();
    FILE *fp;
    Histogram *h;
    char *filename = "data/image.txt";
    int map_y[MAX] = {[0 ... MAX-1] = 0};

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Daten kann nicht geoeffnet werden\n");
        exit(EXIT_FAILURE);
    }
    h= getHistogram(fp);
    normalize(h);
    
    printf("\n");
    int counter = ROW + 1;

    for (int i = 0; i <= ROW ; i++){
        for (int j = 0; j <= COL ; j++){
            if(i == ROW  && j == 0){
                printf("0" );
            } else if (i==0 && j ==0){
                printf("%d ", h->max_h);
            }else if (i==ROW && j == COL+1){
                printf("  %d ", COL);
            }
            else if ( j == 0 || j == COL){
                printf("|");
            }else if ( i == ROW){
                printf("-");
            }else if (h->data[j-1] == counter-1){
                printf("*");
                h->data[j-1]--;
            }else{
                printf(" ");
            }
        }
        counter--;
        printf("\n");
    }
    free(fp);
    free(h);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // Die verstrichene Zeit berechnen
    printf("EXECUTION TIME %.3f seconds\n", time_taken);
    return 0;
}



Histogram *getHistogram(FILE *file) {
    int m = 0;
    int n = 0;
    size_t len = 0;
    char *line = NULL;
    char *tmp = NULL;
    int *hist = calloc(MAX, sizeof(int));
    // Histogram *result = INIT_HISTOGRAM;
    Histogram *result = malloc(sizeof(Histogram));
    *result = INIT_HISTOGRAM;

    // Zeilenweise lesen
    while (getline(&line, &len, file) != -1) {
        // Zeile aufteilen
        m++;
        n = 1;
        hist[atoi(strtok(line, SEP))]++;
        while ((tmp = strtok(NULL, SEP))) {
            hist[atoi(tmp)]++;
            n++;
        }
    }
    // Stastistic
    for (int i = 0; i < MAX; ++i) {
        result->sum += hist[i]; //SUM
        result->avg += i* hist[i]; // AVG
        result->variance += i*i* hist[i]; // Variance
        // MIN
        if(result->min == -1 && hist[i] != 0) {
            result->min = i;
        }
        // MAX
        if (hist[i] != 0) {
            result->max = i;
            result->dynamic += 1;
        }
        if (hist[i] > result->max_h){
            result->max_h = hist[i];
            }
        if (hist[i] < result->min_h){
            result->min_h = hist[i];
            }
        
        // Cumulative summe
        result->cumsum[i] =result->sum;    
        // variance and standard variance 
        if(i + 1 == MAX){
            result->variance = (result->variance - result->avg*result->avg/(m*n))/(m*n);
            result->std = sqrt(result->variance);
            result->avg /= m*n;
        }
    }
    result->data = hist;
    print(result);
    return result;
}
void normalize(Histogram *h ){
    int index =0;
    int new_min = INT_MAX;
    int new_max = INT_MIN;
    int tmp[COL] = {[0 ... COL-1] = 0};

    // Qunatisierung
    for (int i = 0; i < MAX; i++){
        index = (int) ((1.0*COL*i)/(float) MAX);
        tmp[index] += h->data[i];
        if (tmp[index] < new_min) new_min = tmp[index];
        if (tmp[index] > new_max) new_max = tmp[index];
    }
    h->max_h = new_max;
    h->min_h = new_min;
    float abstand = new_max - new_min;
    
    // neue Werten zuweisen
    int temp[MAX] = {[0 ... MAX-1]=0};
    h->data = temp;
    for (int i=0; i < COL; i++){ 
        if (tmp[i] != 0) {
        h->data[i] = (int) (float) ROW*((tmp[i]- new_min)/abstand);
        h->data[i] = h->data[i] == 0 ? 1 : h->data[i];
        }
    }
    
}

int *split(char *line, ssize_t length) {
    int result[length];
    int i = 1;
    char *tmp;

    result[0] = atoi(strtok(line, SEP));
    while ((tmp = strtok(NULL, SEP))) {
        result[i++] = atoi(tmp);
    }
}

void print(Histogram *h){
    printf("\n     MIN: %d", h->min);
    printf("\n     MAX: %d", h->max);
    printf("\n     SUM: %d", h->sum);
    printf("\n     AVG: %.3f", h->avg);
    printf("\n     STD: %.3f", h->std);
    printf("\n   MAX_H: %d", h->max_h);
    printf("\n   MIN_H: %d", h->min_h);
    printf("\n DYNAMIC: %d", h->dynamic);
    printf("\nVARIANCE: %.3f\n", h->variance);
}