//
// Created by TEMKENG on 05.08.2021.
//

#ifndef IMAGE_PROCESSING_HISTOGRAM_H
#define IMAGE_PROCESSING_HISTOGRAM_H

#include <climits>
#include "utils.h"

#define MAX 256
#define SEP " "
#define COL 200
#define ROW 50


class Histogram {
public:
    int sum_, max_, min_;
    float avg, std, variance;
    int max_h, min_h;
    int dynamic; // The dynamic
    int *cumsum; // The cumulative sum
    int *data; // The Occurrence of pixels
    int cols, rows;

    Histogram();

    Histogram(char *filename);

    Histogram(int rows, int cols, int channel, const uint8_t *data_);

    void normalize(int r=ROW, int c=COL);

    /*
    * Plot histogram
    */
    void plot(int r=ROW, int c=COL);

    void info() const;

    Histogram &operator=(const Histogram &rhs);

    bool operator==(const Histogram &rhs) const;

    bool operator!=(const Histogram &rhs) const;

    Histogram clone() const;


    static Histogram NULLHISTOGRAM;
};

#endif //IMAGE_PROCESSING_HISTOGRAM_H
