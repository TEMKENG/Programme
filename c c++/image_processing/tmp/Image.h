//
// Created by TEMKENG on 05.08.2021.
//

#ifndef IMAGE_PROCESSING_IMAGE_H
#define IMAGE_PROCESSING_IMAGE_H

#include <iostream>
#include <cstdint>
#include "Histogram.h"

class Image {
private:
    int rows; // The number of rows
    int size; // The size of the image
    int cols; // The length of one row
    int channels; // The number of channels
    uint8_t *data = nullptr;
    uint8_t _min = UINT8_MAX, _max = 0;
    uint8_t new_channel; // The number of channel
    enum Type {
        JPG, PNG, BMP, TGA
    };
    bool first_min_max = true;

    void min_max();

public:

    Image copy();

    Image clone();

    int min();

    int max();

    friend std::ostream &operator<<(std::ostream &os, const Image &image);

    uint8_t *getData() const;

    Histogram getHistogram() const;

    Image toGray();

    void setData(int index, uint8_t value);

    void setData(uint8_t *data, size_t size);

    int rows_() const;

    int cols_() const;

    int size_() const;

    int getChannels() const;


    void printData(size_t length = 15) const;

    Image(int rows, int cols, int channels);

    Image(const char *filename);

    Image(const Image &image);

    Image equalize();

    bool read(const char *filename);

    bool write(const char *filename);

    void autoContrast(uint8_t a_min = 0, uint8_t a_max = 255);

    static Type getType(const char *filename);

    void add(const Image &rhs);

    void minus(const Image &rhs);
    void mul(const Image &rhs);

    Image &operator+(const Image &rhs);

    Image &operator-(const Image &rhs);
    Image &operator*(const Image &rhs);

    Image &operator+=(const Image &rhs);

    Image &operator-=(const Image &rhs);
    Image &operator*=(const Image &rhs);

    bool operator==(const Image &rhs) const;

    bool operator!=(const Image &rhs) const;

    virtual ~Image();


    void bright(int a);

    void contrast(float a);

    Image convolve(const float *kernel, uint8_t r, uint8_t c);

    uint8_t *get(int row, int col);

    uint8_t get_(int row, int col);

    void set_(int row, int col, uint8_t value);

    void set(int row, int col, const uint8_t *values);

};


#endif //IMAGE_PROCESSING_IMAGE_H
