//
// Created by TEMKENG on 08.08.2021.
//

#ifndef IMAGE_PROCESSING_IMAGES_H
#define IMAGE_PROCESSING_IMAGES_H


#include <ctime>
#include <cmath>
#include <climits>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <ostream>

#define MAX 256
#define SEP " "
#define COL 200
#define ROW 50
#define DEBUG(x, prefix) std::cout <<( strcmp(prefix, "") == 0? "Images" : prefix) << "| "  << #x << " = (" << x << ")" << std::endl;

/**
 * Initializes the pointer `buffer` of length `length` with the value 0.
 *
 * @param buffer The pointer to be initialized.
 * @param length The length of buffer.
 */
template<typename T>
void init(T *buffer, size_t length);

/**
 * Create a pointer of type `dst_type` with the length `length` and initialize it with `initial_value` or with value from `pointer` if `pointer` is not null.
 *
 * @param dst_type The type of the pointer to return.
 * @param length The length of the pointer to return.
 * @param initial_value The value with which the pointer should be initialized.
 * @param src_type The type of source pointer.
 * @param pointer The pointer whose values will be used to initialise the new pointer.
 * @return A pointer of type `dst_type`.
 */
void *
init(const char *dst_type, size_t length, double initial_value = 0, const char *src_type = "", void *pointer = nullptr);

/**
 * Copy the values of a pointer to another pointer.
 *
 * @param dst The destination pointer.
 * @param src The source pointer.
 * @param length The length of the pointer.
 */
template<typename T>
void copy_(T *dst, const T *src, size_t length);

/**
 * Displays data in command line.\n
 * The data can be printed on one line or on several lines.
 * @param data Data to be displayed.
 * @param col The number of items to be printed on a line.
 * @param row The number of line.
 */
void print(const int *data, int col = 20, int row = 1);

/**
 * Displays data in command line.\n
 * The data can be printed on one line or on several lines.
 * @param data Data to be displayed.
 * @param col The number of items to be printed on a line.
 * @param row The number of line.
 */
void print(const float *data, int col = 20, int row = 1);

/**
 * Displays data in command line.\n
 * The data can be printed on one line or on several lines.
 * @param data Data to be displayed.
 * @param col The number of items to be printed on a line.
 * @param row The number of line.
 */
void print(const uint8_t *data, int col = 20, int row = 1);

/**
 * Displays data in command line.\n
 * The data can be printed on one line or on several lines.
 * @param data Data to be displayed.
 * @param col The number of items to be printed on a line.
 * @param row The number of line.
 */
void print(const double *data, int col = 20, int row = 1);

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

    Image *grayscale = nullptr;
    Histogram *histogram = nullptr;

public:
    /**
    * Create a black image with `rows` rows and `cols` columns `channels` channels
    * @param n_rows The number of image rows.
    * @param n_cols The number of image columns.
    * @param n_channels The number of image channels.
    */
    Image(int n_rows, int n_cols, int n_channels);

    /**
     * Read and create an image based on a filename.
     * @param filename The image path.
     */
    Image(const char *filename);

    /**
     * Create a new image from the data of another image.
     *
     * @param image The image whose data will be copied
     */
    Image(const Image &image);

    virtual ~Image();

    /**
     * Copy a image.
     * @return  The copy of the image.
     */
    Image copy();

    /**
     * Copy a image.
     * @return  The copy of the image.
     */
    Image clone();
    /**
     * Compute the maximum value of the image.
     *
     * @return The pixel value with the maximum value.
     */
    float max();

    /**
     * Compute the minimum value of the image.
     *
     * @return The pixel value with the minimum value.
     */
    float min();


    friend std::ostream &operator<<(std::ostream &os, const Image &image);

    /**
     *
     * @return The image data.
     */
    float *getData() const;

    /**
     *
     * @return The image histogram
     */
    Histogram getHistogram();

    /**
     * Compute the grayscale of the image.\n
     * The mean value of all channels is calculated.
     * @return The black and white image
     * @see gray, brightness
     */
    Image toGray();

    /**
    * Compute the grayscale of the image.\n
    * The mean value of all channels is calculated.
    * @return The black and white image
    * @see toGray, brightness
    */
    Image gray();

    /**
    * Compute the grayscale of the image.\n
    * The mean value of all channels is calculated.
    * @return The black and white image
    * @see toGray, gray
    */
    Image brightness();

    /**
     * Update the image data at the position `index` with the value `value`.
     * @param index The position to update in the data.
     * @param value The new value.
     */
    void setData(int index, float value);

    void binning(uint8_t left, uint8_t right);

    /**
     * Update the image data at the position `index` with the value `value`.
     * @param index The position to update in the data.
     * @param value The new value.
     */
    void setData(float *data, size_t size);

    /**
     *
     * @return The number of rows in the image or the height of the image..
     */
    int rows_() const;

    /**
     *
     * @return The number of rows in the image or the height of the image.
     */
    int height() const;

    /**
     *
     * @return The number of columns in the image or the width of the image.
     */
    int cols_() const;

    /**
         *
         * @return The number of columns in the image or the width of the image.
         */
    int width() const;

    /**
     *
     * @return The size of the image.
     */
    int size_() const;

    /**
     *
     * @return The number of image channels
     */
    int getChannels() const;

    /**
     * Print the image data.
     * @param length  The number of data to be displayed.
     */
    void printData(size_t length = 15) const;


    /**
     * Equalizes the histogram of a (grayscale) image.\n
     * The algorithm normalizes the brightness and increases the contrast of the image.
     * @return The equalized image.
     */
    Image equalize();

    /**
     *Read am image from a path `filename`.
     *
     * @param filename The path to the image to be read.
     * @return True if the image is read and not if it is not.
     */
    bool read(const char *filename);

    /**
     *
     * @param filename The path where the image will be stored.
     * @return True if the image has been written to the file and false otherwise.
     */
    bool write(const char *filename);

    /**
     * Bring all pixel values into range `[a_min, a_max]`.
     *
     * @param a_min The left range of the interval.
     * @param a_max The right range of the interval.
     * @return A image
     */
    Image autoContrast(float a_min = 0, float a_max = 255);

    static Type getType(const char *filename);
    /**
     * Adds `rhs`
     * @param rhs The image to be added.
     */
    void add(const Image &rhs);
    /**
     * Adds two images.
     * @param a
     * @param b
     * @return The sum of the two images.
     */
    static Image add(const Image &a, const Image &b);

    float at(int row, int col, uint8_t color = 0);
    
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

    void bright(int a);

    Image contrast(float a = 1);

    Image convolve(const float *kernel, uint8_t r, uint8_t c);

    static Image convolve(Image &image, const float *kernel, uint8_t r, uint8_t c);

    Image clamping(float low, float high);

    /**
     * Get the pixel values at the position `col` and `row`.
     * @param row The number of rows.
     * @param col The number of columns.
     * @return The pixel values at the position `(row, col)`
     */
    float *getPixel(int row, int col);


    void set_at(int row, int col, float value, uint8_t color = 0);

    void setPixel(int row, int col, const float *values);

    Image remove_n_last_col(int n);

    Image resize(int row, int col);


    Image luminance(float r = 1.0 / 3, float g = 1.0 / 3, float b = 1.0 / 3);

    Image threshold(float thresh);

    Image invert();

    void gamma();

    Image sobel();

    Image blur(int kernel_size = 3);

    void norm(float low = 0, float high = 255);

    Image gauss(int radius = 3, float sigma = 0.0);

    template<typename T>
    static void normalize(T *_data, int size, float low = 0, float high = 255);

    Image add_padding(int _row, int _col);

    Image remove_padding(int _row, int _col);

    /**
     * Remove `n` columns from the image with seam carving algorithm.
     * @param n The number of columns to remove from the image.
     * @return The shrinking image.
     */
    Image shrink_n(int n = 50);

    /**
     * Show the path with the minimum energy.
     */
    void show_seam();

    Image flip_h();

    Image flip_v();

    Image rot180();

    Image rot90();

    Image concat_h(Image &image);

    Image concat_v(Image &image);
};

typedef struct _node {
    float value;
    struct _node *left, *right, *l, *r, *u, *next;
} Node;

class Tree : public Node {
public:
    Tree();

    Tree(float value);

    Tree(Image image);

    friend std::ostream &operator<<(std::ostream &os, const Tree &tree);

};

Tree *new_tree(float initial_value = 0.0);

#endif //IMAGE_PROCESSING_IMAGES_H
