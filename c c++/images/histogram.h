//
// Created by TEMKENG on 18.11.2021.
//

#ifndef IMAGES_HISTOGRAM_H
#define IMAGES_HISTOGRAM_H

#include "utils.h"

class Histogram {
public:
    int sum_, max_, min_;
    float avg, std, variance;
    int max_h, min_h;
    int dynamic; // The dynamic
    int *cumsum; // The cumulative sum
    int *data; // The Occurrence of pixels
    int cols, rows;

    /**
     * Create a histogram with default parameter.
     */
    Histogram();

    /**
     * Create a histogram based on data stored in filename.
     * @param filename The filename.
     */
    Histogram(char *filename);

    /**
     * Create a histogram from data `data_`.
     *
     * @param rows The number of rows.
     * @param cols The number of cols
     * @param channel The number of channels.
     * @param data_ The data for which we want to display the histogram
     */
    Histogram(int rows, int cols, int channel, const uint8_t *data_);

    /**
     * Normalize the histogram data
     *
     * @param height The height of the histogram.
     * @param width The width of the histogram.
     */
    void normalize(int height = ROW, int width = COL);

    /**
     * Plot the histogram
     * @param height The height of the histogram.
     * @param width The width of the histogram.
     */
    void plot(int height = ROW, int width = COL);

    /**
     * Print information about the histogram.
     */
    void info() const;

    Histogram &operator=(const Histogram &rhs);

    bool operator==(const Histogram &rhs) const;

    bool operator!=(const Histogram &rhs) const;

    /**
     * Clone the histogram
     * @return The copy of the histogram.
     */
    Histogram clone() const;

};


#endif //IMAGES_HISTOGRAM_H
