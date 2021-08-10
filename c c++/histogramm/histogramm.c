#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "histogramm.h"


__attribute__((unused)) void printList(const int *list, size_t length) {
    printf("\n");
    for (int i = 0; i < length; i++) {
        printf("%i ", list[i]);
    }

}

int main() {
    clock_t t = clock();
    Histogram *h;
    char *filename = "data/image.txt";
    h = getHistogram(filename);
    normalize(h);
    plot(h);
    free(h);
    t = clock() - t;
    double time_taken = ((double) t) / CLOCKS_PER_SEC; // Die verstrichene Zeit berechnen
    printf("Ausfuehrungszeit %.3f Sekunde\n", time_taken);

    return 0;
}


Histogram *getHistogram(char *filename) {
    float m = 0;
    float n = 0;
    size_t len = 1024;
    char line[len];
    char *tmp = NULL;
    int *hist = calloc(MAX, sizeof(int));
    Histogram *result = malloc(sizeof(Histogram));
    *result = INIT_HISTOGRAM; // Histogramm initialisieren
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Daten kann nicht geöffnet werden\n");
        exit(EXIT_FAILURE);
    }
    // Zeilenweise lesen
//    while (getline(&line, &len, file) != -1) {
    while (fgets(line, 1024, file) != NULL) {
        // Zeile aufteilen
        m++;
//        printf("%s", line);
        n = 1;
        hist[atoi(strtok(line, SEP))]++;
        while ((tmp = strtok(NULL, SEP))) {
            hist[atoi(tmp)]++;
            n++;
        }
    }
    // Statistic
    for (int i = 0; i < MAX; ++i) {
        result->sum += hist[i]; //SUM
        result->avg += i * hist[i]; // AVG
        result->variance += i * i * hist[i]; // Variance
        // MIN
        if (result->min == -1 && hist[i] != 0) {
            result->min = i;
        }
        // MAX
        if (hist[i] != 0) {
            result->max = i;
            result->dynamic += 1;
        }
        if (hist[i] > result->max_h) {
            result->max_h = hist[i];
        }
        if (hist[i] < result->min_h) {
            result->min_h = hist[i];
        }

        // Kumulative summe
        result->cumsum[i] = result->sum;
        result->cols = (int) n; // Anzahl von Spalten
        result->rows = (int) m; // Anzahl von Zeilen
        // variance and standard variance
        if (i + 1 == MAX) {
            result->variance = (result->variance - result->avg * result->avg / (m * n)) / (m * n);
            result->std = sqrtf(result->variance);
            result->avg /= m * n;
        }
    }
    printf("M %f N: %f\n", m, n);
    result->data = hist;
    print(result);
    fclose(file);
    return result;
}


void normalize(Histogram *h) {
    int new_index;
    int tmp[COL] = {[0 ... COL - 1] = 0};

    h->max_h = INT_MIN;
    h->min_h = INT_MAX;
    // Quantisierung
    for (int i = 0; i < MAX; i++) {
        new_index = (int) ((1.0 * COL * i) / (float) MAX);
        tmp[new_index] += h->data[i];
        if (tmp[new_index] < h->min_h) h->min_h = tmp[new_index];
        if (tmp[new_index] > h->max_h) h->max_h = tmp[new_index];
    }
    float abstand = (float) (h->max_h - h->min_h);

    // Neue Werten zuweisen
    int temp[MAX] = {0};
    h->data = temp;
    for (int i = 0; i < COL; i++) {
        if (tmp[i] != 0) {
            h->data[i] = (int) ((float) (ROW * (tmp[i] - h->min_h)) / abstand);
            h->data[i] = h->data[i] == 0 ? 1 : h->data[i];
        }
    }
}

void print(Histogram *h) {
    printf("############ Histogramm #########\n");
    printf("\n# %9s %d", "MIN:", h->min);
    printf("\n# %9s %d", "MAX:", h->max);
    printf("\n# %9s %d", "SUM:", h->sum);
    printf("\n# %9s %.3f", "AVG:", h->avg);
    printf("\n# %9s %.3f", "STD:", h->std);
    printf("\n# %9s %d", "MAX_H:", h->max_h);
    printf("\n# %9s %d", "MIN_H:", h->min_h);
    printf("\n# %9s %d", "DYNAMIC:", h->dynamic);
    printf("\n# VARIANCE: %.3f\n", h->variance);
    printf("##################################\n");
//    printf("# Anzahl von Zeilen: %d\n", h->rows);
//    printf("# Anzahl von Spalten: %d\n", h->cols);
//    printf("##################################\n");

}

void plot(Histogram *h) {
    int counter = ROW + 1;
    printf("\n");
    for (int i = 0; i <= ROW; i++) {
        for (int j = 0; j <= COL; j++) {
            if (i == ROW && j == 0) printf("0");
            else if (i == 0 && j == 0) printf("%d ", h->max_h);
            else if (i == ROW && j == COL + 1) printf("  %d ", COL);
            else if (j == 0 or j == COL) printf("|");
            else if (i == ROW) printf("-");
            else if (h->data[j - 1] == counter - 1 and i != 0) {
                printf("*");
                h->data[j - 1]--;
            } else printf(" ");
        }
        counter--;
        printf("\n");
    }

}
