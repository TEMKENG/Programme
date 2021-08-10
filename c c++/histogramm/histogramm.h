#ifndef HISTOGRAM_
#define HISTOGRAM_
//Bibliothek für Limite
#include <limits.h>

#define MAX 256
#define SEP " "
#define ROW 50
#define COL 200
#define and &&
#define or ||
typedef struct Histogram_ {
    int sum;
    int max;
    int min;
    float avg;
    float std;
    float variance;
    int max_h;
    int min_h;
    int dynamic;
    int *cumsum;
    int *data;
    int cols, rows;

} Histogram;

int ptr[MAX] = {0};
int data[MAX] = {0};
// int ptr[MAX] = {[0 ... MAX-1] = 0};
// int data[MAX] = {[0 ... MAX-1] = 0};
Histogram INIT_HISTOGRAM = {.sum=0, .max=0, .min=-1, .avg=0, .std=0, .variance=0, .max_h=INT_MIN, .min_h= INT_MAX,
        .dynamic=0, .cumsum= ptr, .data=data, .cols=0, .rows=0};

/*
 * Histogramm plotten
 */
void plot(Histogram *h);

/*
 * Gibt fast alle Eigenschaft des Histogramms `h`
 */
void print(Histogram *h);

/*
 * Normalisiere das Histogramm
 * Die Werte des Histogramm werden in `[0, COL]´ und `[0, ROW]`
 * @param h Der Histogramm
 */
void normalize(Histogram *h);

Histogram *getHistogram(char *filename);

#endif