//
// Created by TEMKENG on 08.08.2021.
//

#ifndef IMAGE_PROCESSING_IMAGES_H
#define IMAGE_PROCESSING_IMAGES_H


#include <ctime>
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

void *init(const char *type, size_t length, const char *src_type = "", void *pointer = nullptr);

void init(uint8_t *values, size_t length);

void copy_(int *dst, const int *src, size_t length);

void copy_(float *dst, const float *src, size_t length);

void copy_(uint8_t *dst, const uint8_t *src, size_t length);

void print(const int *data, int length = 20);

void print(const float *data, int length = 20);

void print(const double *data, int length = 20);

void print(const uint8_t *data, int length = 20);

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
    int rows{}; // The number of rows
    int size; // The size of the image
    int cols{}; // The length of one row
    int channels{}; // The number of channels
    float *data = nullptr;
    float _min = UINT16_MAX, _max = 0;
    uint8_t new_channel; // The number of channel
    enum Type {
        JPG, PNG, BMP, TGA
    };
    bool first_min_max = true;

    void min_max();

public:

    Image copy();

    Image clone();

    float min();

    float max();

    friend std::ostream &operator<<(std::ostream &os, const Image &image);

    float *getData() const;

    Histogram getHistogram() const;

    Image toGray();

    void setData(int index, float value);

    void setData(float *data, size_t size);

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

    void autoContrast(float a_min = 0, float a_max = 255);

    static Type getType(const char *filename);

    void add(const Image &rhs);

    static Image add(const Image &a, const Image &b);

    static Image minus(const Image &a, const Image &b);

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

    static Image convolve(Image &image, const float *kernel, uint8_t r, uint8_t c);

    float *get(int row, int col);

    float get_(int row, int col);

    void set_(int row, int col, float value);

    void set(int row, int col, const float *values);

//    void binning(uint8_t left, uint8_t right);
    Image gray();

    Image brightness();

    Image clamping(float low, float high);

//    Image luminance(float r = 1.0 / 3, float g = 1.0 / 3, float b = 1.0 / 3);
    Image luminance(float r, float g, float b);

    Image threshold(float thresh);

    Image invert();

    void gamma();

    Image sobel();

    void norm(float low = 0, float high = 255);

    void Gauss();
    //    Separiert bar

    static void normalize(float *_data, int size, float low = 0, float high = 255);

    static void normalize(double *_data, int size, double low = 0, double high = 255);

    static void normalize(uint8_t *_data, int size, uint8_t low = 0, uint8_t high = 255);

    static void normalize(uint16_t *_data, int size, uint16_t low = 0, uint16_t high = 255);


    Image add_padding(int _row, int _col);
    Image remove_padding(int _row, int _col);
};




#endif //IMAGE_PROCESSING_IMAGES_H
