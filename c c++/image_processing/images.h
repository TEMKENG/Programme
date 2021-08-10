//
// Created by TEMKENG on 08.08.2021.
//

#ifndef IMAGE_PROCESSING_IMAGES_H
#define IMAGE_PROCESSING_IMAGES_H


#include <climits>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <ostream>

#define MAX 256
#define SEP " "
#define COL 200
#define ROW 50


void init(int *buffer, size_t length);

void *init(const char *type, size_t length, void *pointer = nullptr);

void init(uint8_t *values, size_t length);

void copy_(int *dst, const int *src, size_t length);

void copy_(uint8_t *dst, const uint8_t *src, size_t length);

void print(const int *data, int length);

void print(const float *data, int length);

void print(const uint8_t *data, int length);

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

    void normalize(int r = ROW, int c = COL);

    /*
    * Plot histogram
    */
    void plot(int r = ROW, int c = COL);

    void info() const;

    Histogram &operator=(const Histogram &rhs);

    bool operator==(const Histogram &rhs) const;

    bool operator!=(const Histogram &rhs) const;

    Histogram clone() const;


    static Histogram NULLHISTOGRAM;
};

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

    static Image add(const Image &a, const Image &b);

    void minus(const Image &rhs);

    void mul(const Image &rhs);

    Image operator+(const Image &rhs) const;

    Image operator-(const Image &rhs) const;

    Image operator*(const Image &rhs) const;

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

//    void binning(uint8_t left, uint8_t right);
    Image gray();

    Image brightness();

    Image clamping(uint8_t low, uint8_t high);

    Image luminance(float r = 1.0 / 3, float g = 1.0 / 3, float b = 1.0 / 3);

    Image threshold(float thresh);

    Image invert();

    void gamma();

    void Sobel();

    void Gauss();
    //    Separiert bar

};


#endif //IMAGE_PROCESSING_IMAGES_H
