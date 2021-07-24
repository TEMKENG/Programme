#ifndef HISTOGRAM_
#define HISTOGRAM_
//Bibliothek für Limite
#include <limits.h>

#define MAX 256
#define SEP " "
#define ROW 95
#define COL 150

typedef struct  Histogram_ {
    int sum;
    int max ;
    int min ;
    float avg;
    float std;
    float variance;
    int max_h;
    int min_h;
    int dynamic;
    int* cumsum;
    int* data;
} Histogram;

// int ptr[MAX] = {[0 ... MAX-1] = 0};
// int data[MAX] = {[0 ... MAX-1] = 0};
int ptr[MAX] = {0};
int data[MAX] = {0};
Histogram INIT_HISTOGRAM  = {0,0,-1,0,0,0, INT_MIN, INT_MAX, 0, ptr, data};

void print(Histogram *h);
void normalize(Histogram *h);
#endif