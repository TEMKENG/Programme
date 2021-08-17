//
// Created by TEMKENG on 08.08.2021.
//


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"
#include "images.h"


// UTILS
void *init(const char *dst_type, size_t length, double initial_value, const char *src_type, void *pointer) {

    if (strcmp(dst_type, "int") == 0) {
        int *buffer = new int[length];
        if (strcmp(src_type, "int") == 0) {
            int *cpy = static_cast<int * >(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? int(initial_value) : cpy[i];
            }
        } else {
            uint8_t *cpy = static_cast<uint8_t *>(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (uint8_t) initial_value : cpy[i];
            }
        }
        return buffer;
    } else if (strcmp(dst_type, "uint8_t") == 0) {
        uint8_t *buffer = new uint8_t[length];
        if (strcmp(src_type, "float") == 0) {
            float *cpy = static_cast<float * >(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (uint8_t) initial_value : (uint8_t) lround(cpy[i]);
            }
        } else {
            uint8_t *cpy = static_cast<uint8_t *>(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (uint8_t) initial_value : cpy[i];
            }
        }
        return buffer;
    } else if (strcmp(dst_type, "uint16_t") == 0) {
        uint16_t *buffer = new uint16_t[length];
        if (strcmp(src_type, "uint16_t") == 0) {
            uint16_t *cpy = static_cast<uint16_t * >(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (uint16_t) initial_value : cpy[i];
            }
        } else {
            uint8_t *cpy = static_cast<uint8_t *>(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (uint8_t) initial_value : (uint16_t) cpy[i];
            }
        }
        return buffer;
    } else if (strcmp(dst_type, "float") == 0) {
        float *buffer = new float[length];
        if (strcmp(src_type, "float") == 0) {
            float *cpy = static_cast<float * >(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (float) initial_value : cpy[i];
            }
        } else {
            uint8_t *cpy = static_cast<uint8_t *>(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? (float) initial_value : (float) cpy[i];
            }
        }

        return buffer;
    } else if (strcmp(dst_type, "double") == 0) {
        double *buffer = new double[length];
        if (strcmp(src_type, "double") == 0) {
            double *cpy = static_cast<double * >(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? initial_value : cpy[i];
            }
        } else {
            uint8_t *cpy = static_cast<uint8_t *>(pointer);
            for (int i = 0; i < length; ++i) {
                buffer[i] = cpy == nullptr ? initial_value : (double) cpy[i];
            }
        }
        return buffer;
    }

    return nullptr;
}


template<typename T>
void init(T *buffer, size_t length) {
    buffer = new T[length];
    for (int i = 0; i < length; ++i) {
        buffer[i] = 0;
    }
}

template<typename T>
void copy_(T *dst, const T *src, size_t length) {
    for (int i = 0; i < length; ++i) {
        dst[i] = src[i];
    }
}

void print(const int *data, int col, int row) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d ", data[i * row + j]);
        }
        printf("\n");
    }
}

void print(const uint8_t *data, int col, int row) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d ", data[i * row + j]);
        }
        printf("\n");
    }
}

void print(const float *data, int col, int row) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%lf ", data[i * row + j]);
        }
        printf("\n");
    }
}

void print(const double *data, int c, int r) {
    for (int i = 0; i < r; ++i) {
        for (int j = 0; i < c; ++i) {
            printf("%.3lf ", data[i * r + j]);
        }
        printf("\n");
    }
}

// HISTOGRAM

Histogram::Histogram() : sum_(0), max_(0), min_(-1), avg(0), std(0.0), cols(0), rows(0), dynamic(0), variance(0),
                         max_h(INT_MIN), min_h(INT_MAX) {
    data = static_cast<int *>(init("int", MAX));
    cumsum = static_cast<int *>(init("int", MAX));
}

Histogram::Histogram(char *filename) {

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
    copy_(_copy.data, data, 256);
    copy_(_copy.cumsum, cumsum, 256);
    return _copy;

}

// IMAGE

Image::~Image() {
    free(data);
}

Image::Image(const char *filename) {
    if (read(filename)) {
        printf("File %s is read\n", filename);
        size = channels * rows * cols;
        new_channel = channels > 3 ? 3 : channels;
        min_max();
    } else
        printf("File %s was not read\n", filename);
}

Image::Image(int _rows, int _cols, int _channels) : rows(_rows), cols(_cols), channels(_channels) {
    size = channels * rows * cols;
    new_channel = channels > 3 ? 3 : channels;
//    data = static_cast<uint8_t *>(init("uint8_t", size));
    data = static_cast<float *>(init("float", size));
    min_max();
}

Image::Image(const Image &image) : rows(image.rows), cols(image.cols), channels(image.channels) {
    size = image.size_();
    data = static_cast<float *>(init("float", size, 0, "float", image.data));
    new_channel = image.new_channel;
    min_max();
}

bool Image::read(const char *filename) {
    uint8_t *tmp = stbi_load(filename, &cols, &rows, &channels, 0);
    size = cols * rows * channels;
    data = static_cast<float *>(init("float", size, 0, "uint8_t", tmp));
    return data != nullptr;
}

bool Image::write(const char *filename) {
    int check;
    auto *cpy = static_cast<float *>(init("float", size, 0, "float", data));
    if (max() > 255 or min() < 0) {
        normalize(cpy, size);
    }
    auto *_data = static_cast<uint8_t *>(init("uint8_t", size, 0, "float", cpy));
    switch (getType(filename)) {
        case PNG:
            check = stbi_write_png(filename, cols, rows, channels, _data, cols * channels);
            break;
        case JPG:
            check = stbi_write_jpg(filename, cols, rows, channels, _data, 100);
            break;
        case BMP:
            check = stbi_write_bmp(filename, cols, rows, channels, _data);
            break;
        case TGA:
            check = stbi_write_tga(filename, cols, rows, channels, _data);
            break;
    }

    return check != 0;
}

Image::Type Image::getType(const char *filename) {
    const char *extension = strrchr(filename, '.');
    if (extension == nullptr) return PNG;
    if (strcmp(extension, ".jpg") == 0 or strcmp(extension, ".jpeg") == 0) return JPG;
    else if (strcmp(extension, ".bmp") == 0) return BMP;
    else if (strcmp(extension, ".tga") == 0) return TGA;
    return PNG;
}

/**
 *
 * @return  The image data.
 */
float *Image::getData() const {
    return data;
}

/**
 *
 * @return The number of rows of the image.
 */
int Image::rows_() const {
    return rows;
}

/**
 *
 * @return The number of columns of the image.
 */
int Image::cols_() const {
    return cols;
}

/**
 *
 * @return The number of channels of the image.
 */
int Image::getChannels() const {
    return channels;
}

void Image::setData(int index, float value) {
    data[index] = value;
}

Image Image::toGray() {
    if (grayscale == nullptr) {
        if (new_channel == 1) {
            grayscale = new Image(*this);
            return *grayscale;
        }
        int index = 0;
        float pixelSomme;
        Image _gray(rows, cols, 1);
        for (int i = 0; i < size; i += channels) {
            pixelSomme = 0;
            for (int j = 0; j < new_channel; j++) { pixelSomme += data[i + j]; }
            _gray.setData(index++, pixelSomme / (float) new_channel);
        }
        grayscale = new Image(_gray);
    }
    return *grayscale;
}

Image Image::gray() {
    return toGray();
}

Image Image::brightness() {
    return toGray();
}

void Image::setData(float *data_, size_t length) {
    memcpy(Image::data, data_, length);
}

void Image::printData(const size_t length) const {
    for (int i = 0; i < length; i++) {
        printf("%.f ", data[i]);
    }
    printf("\n");
}

Histogram Image::getHistogram() {
    if (histogram == nullptr) {
        uint8_t *tmp_data = static_cast<uint8_t *>(init("uint8_t", size, 0, "float", data));
        histogram = new Histogram(rows, cols, channels, tmp_data);
    }
    return *histogram;
}

float Image::min() {
    if (first_min_max)
        min_max();
    return _min;
}

float Image::max() {
    if (first_min_max)
        min_max();
    return _max;
}

void Image::min_max() {
    for (int i = 0; i < size; ++i) {
        if (data[i] > _max) _max = data[i];
        if (data[i] < _min) _min = data[i];
    }
    first_min_max = false;
}

int Image::size_() const {
    return size;
}

Image Image::autoContrast(float a_min, float a_max) {
    Image cpy = clone();
    float dist = a_max - a_min;
    float dist_ = max() - min();
    if (dist == 0 or dist_ == 0)
        return cpy;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            cpy.data[i + j] = a_min + (data[i + j] - _min) * dist / dist_;
        }
    }
    cpy._min = a_min;
    cpy._max = a_max;
    return cpy;
}

Image Image::equalize() {
    float mn = (float) size;
    Histogram hist = getHistogram();
    Image result = copy();

    for (int i = 0; i < size; i += new_channel) {
        for (int j = 0; j < new_channel; ++j) {
            result.data[i + j] = ((float) hist.cumsum[(uint8_t) data[i + j]] * max()) / mn;
        }
    }

    return result;
}

void Image::bright(int a) {
//    float brightness;
    const float offset = a;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            data[i + j] += offset;
//            data[i + j] = brightness > 255 ? 255 : (brightness < 0 ? 0 : (uint8_t) brightness);
        }
    }
}

Image Image::contrast(float alpha) {
    Image result = clone();
    if (alpha < 0) return result;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            result.data[i + j] *= alpha;
        }
    }
    return result;
}

Image Image::convolve(const float *kernel, uint8_t r_, uint8_t c_) {
    float sum;
    int index = 0;
    Image cpy = clone();
//    auto *data_result = static_cast<float *>(init("float", size, 0, "float", cpy.data));
    uint8_t l = r_ / 2, m = c_ / 2;
    for (int color = 0; color < new_channel; ++color) {
        for (int i = l; i < rows - l; ++i) {
            for (int j = m; j < cols - m; ++j) {
                sum = 0;
                for (int row = -l; row <= l; ++row) {
                    for (int col = -m; col <= m; ++col) {
                        sum += get_(i + row, j + col, color) * kernel[(row + l) * c_ + col + m];
                    }
                }
                cpy.set_(i, j, sum, color);
            }
        }
    }
    return cpy;
}

Image Image::copy() {
    Image cpy(rows, cols, channels);
    copy_(cpy.data, data, size);
    cpy.min_max();
    return cpy;
}

Image Image::clone() {
    return copy();
}

float Image::get_(int row, int col, uint8_t color) {
    if ((row < 0 or row >= rows) or (col < 0 or col >= cols))
        return INFINITY;
    return data[(row * cols + col) * channels + color];
}

float *Image::get(int row, int col) {
    auto pixels = static_cast<float *>(init("float", channels));
    int offset = (row * cols + col) * channels;
    for (int i = 0; i < channels; ++i) {
        pixels[i] = data[offset + i];
    }
    return pixels;
}

void Image::set_(int row, int col, float value, uint8_t color) {
    data[(row * cols + col) * channels + color] = value;
}

void Image::set(int row, int col, const float *values) {
    if (values == nullptr) return;
    int offset = (row * cols + col) * channels;
//    printf("OFFSET: %d col: %d row: %d\n", offset, col, row);
    for (int i = 0; i < channels; ++i) {
        data[offset + i] = values[i];
    }
}

void Image::minus(const Image &rhs) {
    uint16_t value;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            value = data[i + j] - (uint16_t) rhs.data[i + j];
            data[i + j] = value < 0 ? 0 : (uint8_t) value;
        }
    }
}

void Image::add(const Image &rhs) {
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            data[i + j] = data[i + j] + rhs.data[i + j];
        }
    }
}

void Image::mul(const Image &rhs) {
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            data[i + j] = data[i + j] * rhs.data[i + j];
        }
    }
}

Image Image::operator+(const Image &rhs) const {
    Image plus(rows, cols, channels);
    plus += *this;
    plus += rhs;
    return plus;
}

Image Image::operator-(const Image &rhs) const {
    Image sub(rows, cols, channels);
    sub += *this;
    sub -= rhs;
    return sub;
}

Image Image::operator*(const Image &rhs) const {
    Image multiply(rows, cols, channels);
    multiply += *this;
    multiply *= rhs;
    return multiply;
}

Image &Image::operator+=(const Image &rhs) {
    add(rhs);
    return *this;
}

Image &Image::operator-=(const Image &rhs) {
    minus(rhs);
    return *this;
}

Image &Image::operator*=(const Image &rhs) {
    mul(rhs);
    return *this;
}

bool Image::operator==(const Image &rhs) const {
    return data == rhs.data &&
           rows == rhs.rows &&
           cols == rhs.cols &&
           channels == rhs.channels;
}

bool Image::operator!=(const Image &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Image &image) {
    os << "Rows: " << image.rows << " Columns: " << image.cols << " Channels: " << image.channels;
    return os;
}

Image Image::luminance(float r, float g, float b) {
//    printf("R: %f G: %f B: %f\n", r, g, b);
//    std::cout << "Luminance:\n" << *this << std::endl;
    if (new_channel < 3) { return toGray(); }
    Image lum = copy();
//    Image lum(rows, cols, channels);
    for (int i = 0; i < size; i += channels) {
        lum.data[i] *= r;
        lum.data[i + 1] *= g;
        lum.data[i + 2] *= b;
    }
//    lum.write("data/luminance.png");
    return lum;
}

Image Image::clamping(float low, float high) {
    float pixel_value;
    Image clamp = copy();
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; j++) {
            pixel_value = data[i + j];
            clamp.data[i + j] = pixel_value > high ? high : (pixel_value < low ? low : pixel_value);
        }
    }
    return clamp;
}

Image Image::threshold(float thresh) {
    Image result(rows, cols, channels);
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; j++) {
            result.data[i + j] = (uint8_t) (data[i + j] > thresh ? 255 : 0);
        }
        if (new_channel < channels) result.data[i + new_channel] = data[i + new_channel];
    }
    return result;
}

Image Image::invert() {
    min_max();
    Image invert_ = copy();
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; j++) {
            invert_.data[i + j] = _max - data[i + j];
        }
    }
    return invert_;
}

void Image::gamma() {

}

Image Image::sobel() {
    float u[] = {1, 2, 1};
    float v[] = {-1, 0, 1};
    float kernel_x[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    float kernel_y[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    Image _blur = add_padding(3, 3).blur(3);

//    Image s_x = _blur.convolve(u, 3, 1).convolve(v, 1, 3);
//    Image s_y = _blur.convolve(v, 3, 1).convolve(u, 1, 3);

    Image s_x = _blur.convolve(kernel_x, 3, 3).remove_padding(3, 3);
    Image s_y = _blur.convolve(kernel_y, 3, 3).remove_padding(3, 3);
//    s_x.write("data/sobel_x.png");
//    s_y.write("data/sobel_y.png");
    Image _sobel(rows, cols, channels);
    for (int i = 0; i < size; ++i) {
        _sobel.data[i] = std::sqrt(s_x.data[i] * s_x.data[i] + s_y.data[i] * s_y.data[i]);
    }
//    _sobel.write("data/sobel_xy.png");
    return _sobel.toGray();
}

Image Image::gauss(int radius, float sigma) {
    float value;
    int length = radius * radius;
    float *kernel = static_cast<float *>(init("float", length));
    sigma = sigma == 0.0 ? float(0.3 * ((radius - 1) * 0.5 - 1) + 0.8) : sigma;
    sigma *= 2 * sigma;
    int i = 0, j = 0;
    for (int x = -radius / 2; x <= 0; ++x) {
        j = 0;
        for (int y = -radius / 2; y <= 0; ++y) {
            value = exp(-float(x * x + y * y) / sigma);
            kernel[i * radius + j] = value;
            kernel[(i + 1) * radius - 1 - j] = value;
            kernel[(radius - i - 1) * radius + j] = value;
            kernel[(radius - i) * radius - j - 1] = value;
            ++j;
        }
        i++;
    }
    value = 0;
//    Compute the sum
    for (i = 0; i < length; ++i) {
        value += kernel[i];
    }
//    Normalize the kernel
    for (i = 0; i < length; ++i) {
        kernel[i] /= value;
    }

//    print(kernel, radius, radius);
    return convolve(kernel, radius, radius);
}

Image Image::add(const Image &a, const Image &b) {
    Image result(a.rows, a.cols, a.channels);
    result += a;
    result += b;
    return result;
}

Image Image::minus(const Image &a, const Image &b) {
    Image result(a.rows, a.cols, a.channels);
    result += a;
    result -= b;
    return result;
}

Image Image::convolve(Image &image, const float *kernel, uint8_t r, uint8_t c) {
//    Image conv = image.convolve(kernel, r, c);
    return image.convolve(kernel, r, c);
}

void Image::norm(float low, float high) {
    normalize(data, size, low, high);
}

template<typename T>
void Image::normalize(T *_data, int size, float low, float high) {
    T max_ = _data[0], min_ = _data[0];
    for (int i = 1; i < size; ++i) {
        if (_data[i] > max_) max_ = _data[i];
        if (_data[i] < min_) min_ = _data[i];
    }
    T dist = max_ - min_;
    T high_low = high - low;
    if (dist == 0) return;
    for (int i = 0; i < size; i++) {
        _data[i] = high_low * (_data[i] - min_) / dist;
    }
}

Image Image::add_padding(int _row, int _col) {
    Image zp(rows + 2 * _row, cols + 2 * _col, channels);
    int index, offset;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            index = (i * cols + j) * channels;
            offset = ((i + _row) * zp.cols + j + _col) * channels;
            for (int k = 0; k < channels; ++k) {
                zp.data[offset + k] = data[index + k];
            }
        }
    }
    return zp;
}

Image Image::remove_padding(int _row, int _col) {
    Image zp(rows - 2 * _row, cols - 2 * _col, channels);
    int index, offset;
    for (int i = _row; i < rows - _row; ++i) {
        for (int j = _col; j < cols - _col; ++j) {
            index = (i * cols + j) * channels;
            offset = ((i - _row) * zp.cols + j - _col) * channels;
            for (int k = 0; k < channels; ++k) {
                zp.data[offset + k] = data[index + k];
            }
        }
    }
    return zp;
}

Image Image::blur(int ksize) {
    float *kernel = static_cast<float *>(init("float", ksize * ksize, 1. / float(ksize * ksize)));

    return clone().convolve(kernel, ksize, ksize);
}

template<typename T>
uint8_t get_min_pos(T *array, size_t length) {
    uint8_t pos = 0;
    T min_value = array[0];
    for (int i = 1; i < length; ++i) {
        if (array[i] < min_value) {
            min_value = array[i];
            pos = i;
        }
    }
    return pos;

}

template<typename T>
uint8_t get_max_pos(T *array, size_t length) {
    uint8_t pos = 0;
    T max_value = array[0];
    for (int i = 1; i < length; ++i) {
        if (array[i] > max_value) {
            max_value = array[i];
            pos = i;
        }
    }
    return pos;

}

Image Image::seam_carving() {
    uint8_t pos;
    float seam_color[] = {255, 0, 0};
    float pixel_value, left, right, under;
    float *lur = static_cast<float *>(init("float", 3, 0.0)); // left, right, under;
//    Image cpy = toGray();
    Image energy = gray().sobel();
    Image positions(energy.rows, energy.cols, energy.channels);
    printf("%f\n", get_(1, 0) > get_(-1, 0));
    for (int i = rows - 2; i > -1; --i) {
//    for (int i = rows - 2; i > rows / 3; --i) {
        for (int j = 0; j < cols; ++j) {
            lur[0] = energy.get_(i + 1, j - 1);
            lur[1] = energy.get_(i + 1, j);
            lur[2] = energy.get_(i + 1, j + 1);
            pos = get_min_pos(lur, 3);
            energy.set_(i, j, lur[pos] + energy.get_(i, j));
            positions.set_(i, j, pos);
        }
    }
    int pos_ = get_min_pos(energy.data, energy.cols);
    printf("Position: %d\n", pos_ * 3);

    for (int i = 0; i < rows; ++i) {
        set(i, pos_, seam_color);
        pos_ = pos_ - 1 + (int) positions.get_(i, pos_);
    }
    DEBUG(pos_);
    energy.write("data/energy.png");
    positions.write("data/positions.png");
    write("data/seam.png");
//    return energy;
    return positions;
}

Tree *new_tree(float initial_value) {
    Tree *m = static_cast<Tree *>(malloc(sizeof(Tree)));
    if (m == nullptr) return nullptr;
    m->left = nullptr;
    m->right = nullptr;
    m->l = nullptr;
    m->r = nullptr;
    m->u = nullptr;
    m->value = initial_value;
    return m;
}


// TREE

Tree::Tree() {
    this->left = nullptr;
    this->right = nullptr;
    this->l = nullptr;
    this->r = nullptr;
    this->u = nullptr;
    this->value = 0;
}

Tree::Tree(float value) : Tree() {
    this->value = value;
}

Tree::Tree(Image image) {
    Tree *trees = new Tree[image.size_()];
    float *data = image.getData();
    int col = 0, row = 0;
    for (int i = 0; i < image.rows_() ; i++) {
        for (int j = 0; j < image.cols_(); j++) {
            trees[row + j].value = image.get_(i, j);
            if (j != 0) {
                trees[row + j].left = &trees[row + j - 1];
                if (i != image.rows_() - 1) {
                    trees[row + j].l = &trees[row + image.cols_() + j - 1];
                }
            }
            if (j + 1 != image.cols_()) {
                trees[row + j].right = &trees[row + j + 1];
                if (i != image.rows_() - 1) {
                    trees[row + j].r = &trees[row + image.cols_() + j + 1];
                }
            }
            if (i != image.rows_() - 1) {
                trees[row + j].u = &trees[row + image.cols_() + j];
            }

        }
        row += image.cols_();
    }

//    image.printData(10);
//    for (int i = 0; i < 10; i++) {
//        std::cout << trees[i];
//    }
//    col = 50;
//    for (int i = 0; i < col; i++) {
//        for (int j = 0; j < 2*col; ++j)
//            std::cout << image.get_(i, j) << " ";
//        printf("\n");
//    }
//
//    printf("\n");
}

std::ostream &operator<<(std::ostream &os, const Tree &tree) {
    os << "L: " << ((tree.left == nullptr) ? INFINITY : tree.left->value)
       << " V: " << tree.value
       << " R: " << ((tree.right == nullptr) ? INFINITY : tree.right->value)
       << " BL: " << ((tree.l == nullptr) ? INFINITY : tree.l->value)
       << " BU: " << ((tree.u == nullptr) ? INFINITY : tree.u->value)
       << " BR: " << ((tree.r == nullptr) ? INFINITY : tree.r->value)
       << std::endl;
    return os;
}
