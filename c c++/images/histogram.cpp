//
// Created by TEMKENG on 18.11.2021.
//

#include "histogram.h"

Histogram::Histogram() : sum_(0), max_(0), min_(-1), avg(0), std(0.0), cols(0), rows(0), dynamic(0), variance(0),
max_h(INT_MIN), min_h(INT_MAX) {
    data = static_cast<int *>(init("int", MAX));
    cumsum = static_cast<int *>(init("int", MAX));
}

Histogram::Histogram(char *filename):Histogram() {
}

Histogram::Histogram(int _rows, int _cols, int channel, const uint8_t *data_) : Histogram() {
    rows = _rows;
    cols = _cols;
    for (int i = 0; i < rows * cols * channel; i++) {
        sum_ += data_[i]; // SUM
        if (data_[i] > max_) max_ = data_[i]; // MIN
        if (data_[i] < min_) min_ = data_[i]; // MAX
        data[data_[i]]++; // Histogram data
    }
    // Statistic
    for (int i = 0; i < MAX; ++i) {
        avg += i * data[i]; // AVG
        variance += i * i * data[i];
        if (data[i] != 0) dynamic++;
        if (min_ == -1 and data[i] != 0) min_ = i; // Smallest pixel value
        if (data[i] > max_h) max_h = data[i];
        if (data[i] < min_h) min_h = data[i]; // Smallest value of the histogram
        cumsum[i] = i == 0 ? data[i] : data[i] + cumsum[i - 1];
        if (i + 1 == MAX) {
            float mn = (float) rows * (float) cols;
            variance = (variance - avg * avg / mn) / mn; // VARIANCE
            std = sqrtf(variance); // STANDARD VARIANCE
            avg /= mn; // AVERAGE
        }
    }
}

void Histogram::normalize(int height, int width) {
    int new_index;
    max_h = INT_MIN;
    min_h = INT_MAX;
    int *tmp = static_cast<int *>(init("int", width));
    // Quantisation
    for (int i = 0; i < MAX; i++) {
        new_index = (int) ((1.0 * width * i) / (float) MAX);
        tmp[new_index] += data[i];
        if (tmp[new_index] < min_h) min_h = tmp[new_index];
        if (tmp[new_index] > max_h) max_h = tmp[new_index];
    }
    auto contrast = (float) (max_h - min_h);
    // Assign new values
    init(data, MAX);
    for (int i = 0; i < width; i++) {
        if (tmp[i] != 0) {
            data[i] = (int) ((float) (height * (tmp[i] - min_h)) / contrast);
            data[i] = data[i] == 0 ? 1 : data[i];
        }
    }
}

void Histogram::plot(int r, int c) {
    normalize(r, c);
    int counter = r;
    int *data_copy = new int[c];
    copy_(data_copy, data, c);
    printf("\n%d \n", max_h);
    for (int i = 0; i <= r; i++) {
        for (int j = 0; j <= c + 2; j++) {
            if (i == r && j == 0) printf("0");
            else if (i == r && j == c + 2) printf("  %d ", c);
            else if (j == 0 or j == c + 1) printf("|");
            else if (i == r) printf("-");
            else if (data_copy[j - 1] == counter) {
                printf("*");
                data_copy[j - 1]--;
            } else { printf(" "); }
        }
        counter--;
        printf("\n");
    }
}

void Histogram::info() const {
    printf("############ Histogramm #########\n");
    printf("\n# %9s %d", "MIN:", min_);
    printf("\n# %9s %d", "MAX:", max_);
    printf("\n# %9s %d", "SUM:", sum_);
    printf("\n# %9s %.3f", "AVG:", avg);
    printf("\n# %9s %.3f", "STD:", std);
    printf("\n# %9s %d", "MAX_H:", max_h);
    printf("\n# %9s %d", "MIN_H:", min_h);
    printf("\n# %9s %d", "DYNAMIC:", dynamic);
    printf("\n# VARIANCE: %.3f\n", variance);
    printf("##################################\n");
    printf("# Number columns: %d\n", cols);
    printf("# Number of rows: %d\n", rows);
    printf("##################################\n");

}

bool Histogram::operator==(const Histogram &rhs) const {
    return sum_ == rhs.sum_ &&
    max_ == rhs.max_ &&
    min_ == rhs.min_ &&
    avg == rhs.avg &&
    std == rhs.std &&
    variance == rhs.variance &&
    max_h == rhs.max_h &&
    min_h == rhs.min_h &&
    dynamic == rhs.dynamic &&
    cumsum == rhs.cumsum &&
    data == rhs.data &&
    cols == rhs.cols &&
    rows == rhs.rows;
}

bool Histogram::operator!=(const Histogram &rhs) const {
    return !(rhs == *this);
}

Histogram &Histogram::operator=(const Histogram &rhs) {
    if (this != &rhs) {
        avg = rhs.avg;
        std = rhs.std;
        sum_ = rhs.sum_;
        max_ = rhs.max_;
        min_ = rhs.min_;
        rows = rhs.rows;
        cols = rhs.cols;
        data = rhs.data;
        max_h = rhs.max_h;
        min_h = rhs.min_h;
        cumsum = rhs.cumsum;
        dynamic = rhs.dynamic;
        variance = rhs.variance;
    }
    return *this;
}

Histogram Histogram::clone() const {
    Histogram _copy;
    _copy.avg = avg;
    _copy.std = std;
    _copy.sum_ = sum_;
    _copy.max_ = max_;
    _copy.min_ = min_;
    _copy.rows = rows;
    _copy.cols = cols;
    _copy.max_h = max_h;
    _copy.min_h = min_h;
    _copy.dynamic = dynamic;
    _copy.variance = variance;
    copy_(_copy.data, data, MAX);
    copy_(_copy.cumsum, cumsum, MAX);
    return _copy;

}