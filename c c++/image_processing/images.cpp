//
// Created by TEMKENG on 08.08.2021.
//

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include "images.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

// UTILS
void *init(const char *type, size_t length, void *pointer) {
    if (strcmp(type, "int") == 0) {
        int *buffer = new int[length];
        int *copy = static_cast<int *>(pointer);
        for (int i = 0; i < length; ++i) {
            buffer[i] = copy == nullptr ? 0 : copy[i];
        }
        return buffer;
    } else if (strcmp(type, "uint8_t") == 0) {
        auto *buffer = new uint8_t[length];
        auto *copy = static_cast<uint8_t *>(pointer);
        for (int i = 0; i < length; ++i) {
            buffer[i] = copy == nullptr ? 0 : copy[i];
        }
        return buffer;
    }

    return nullptr;
}

void init(int *buffer, size_t length) {
    buffer = new int[length];
    for (int i = 0; i < length; ++i) {
        buffer[i] = 0;
    }
}

void init(uint8_t *buffer, size_t length) {
    buffer = new uint8_t[length];
    for (int i = 0; i < length; ++i) {
        buffer[i] = 0;
    }
}

void copy_(int *dst, const int *src, size_t length) {
    for (int i = 0; i < length; ++i) {
        dst[i] = src[i];
    }
}

void copy_(uint8_t *dst, const uint8_t *src, size_t length) {
    for (int i = 0; i < length; ++i) {
        dst[i] = src[i];
    }
}

void print(const int *data, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

void print(const uint8_t *data, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

void print(const float *data, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%f ", data[i]);
    }
    printf("\n");
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
            variance = (variance - avg * avg / (rows * cols)) / (rows * cols); // VARIANCE
            std = sqrtf(variance); // STANDARD VARIANCE
            avg /= rows * cols; // AVERAGE
        }
    }
}

void Histogram::normalize(int r, int c) {
    int new_index;
    max_h = INT_MIN;
    min_h = INT_MAX;
    int *tmp = static_cast<int *>(init("int", c));
    // Quantisation
    for (int i = 0; i < MAX; i++) {
        new_index = (int) ((1.0 * c * i) / (float) MAX);
        tmp[new_index] += data[i];
        if (tmp[new_index] < min_h) min_h = tmp[new_index];
        if (tmp[new_index] > max_h) max_h = tmp[new_index];
    }
    auto contrast = (float) (max_h - min_h);
    // Assign new values
    init(data, MAX);
    for (int i = 0; i < c; i++) {
        if (tmp[i] != 0) {
            data[i] = (int) ((float) (r * (tmp[i] - min_h)) / contrast);
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
    stbi_image_free(data);
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
    data = static_cast<uint8_t *>(init("uint8_t", size));
    min_max();
}

Image::Image(const Image &image) : rows(image.rows), cols(image.cols), channels(image.channels) {
    size = channels * rows * cols;
    memcpy(data, image.data, size);
    new_channel = channels > 3 ? 3 : channels;
    min_max();
}

bool Image::read(const char *filename) {
    data = stbi_load(filename, &cols, &rows, &channels, 0);
    return data != nullptr;
}

bool Image::write(const char *filename) {
    int check;
    switch (getType(filename)) {
        case PNG:
            check = stbi_write_png(filename, cols, rows, channels, data, cols * channels);
            break;
        case JPG:
            check = stbi_write_jpg(filename, cols, rows, channels, data, 100);
            break;
        case BMP:
            check = stbi_write_bmp(filename, cols, rows, channels, data);
            break;
        case TGA:
            check = stbi_write_tga(filename, cols, rows, channels, data);
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


uint8_t *Image::getData() const {
    return data;
}

int Image::rows_() const {
    return rows;
}

int Image::cols_() const {
    return cols;
}

int Image::getChannels() const {
    return channels;
}

void Image::setData(int index, uint8_t value) {
    data[index] = value;
}

Image Image::toGray() {
    int index = 0, pixelSomme;
    Image _gray(rows, cols, 1);
    for (int i = 0; i < size; i += channels) {
        pixelSomme = 0;
        for (int j = 0; j < new_channel; j++) { pixelSomme += data[i + j]; }
        _gray.setData(index++, pixelSomme / new_channel);
    }
    return _gray;
}

void Image::setData(uint8_t *data_, size_t length) {
    memcpy(Image::data, data_, length);
}

void Image::printData(const size_t length) const {
    for (int i = 0; i < length; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

Histogram Image::getHistogram() const {
    Histogram histogram(rows, cols, channels, data);
    return histogram;
}

int Image::min() {
    if (first_min_max)
        min_max();
    return _min;
}

int Image::max() {
    if (first_min_max)
        min_max();
    return _max;
}

void Image::min_max() {
    for (int i = 0; i < cols * rows * channels; ++i) {
        if (data[i] > _max) _max = data[i];
        if (data[i] < _min) _min = data[i];
    }
    first_min_max = false;
}

int Image::size_() const {
    return size;
}

void Image::autoContrast(uint8_t a_min, uint8_t a_max) {
    uint8_t dist = a_max - a_min;
    uint8_t dist_ = max() - min();
    if (dist == 0 or dist_ == 0)
        return;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            data[i + j] = a_min + (data[i + j] - _min) * dist / dist_;
        }
    }
    _min = a_min;
    _max = a_max;
}

Image Image::equalize() {
    float pixel;
    float mn = size;
    Histogram hist = getHistogram();
    Image result = Image(rows, cols, channels);

    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < channels; ++j) {
            if (j >= new_channel) {
                result.data[i + j] = data[i + j];
                continue;
            }
            pixel = ((float) hist.cumsum[data[i + j]] * max()) / mn;
            result.data[i + j] = pixel > 255 ? 255 : (uint8_t) pixel;
        }
    }

    return result;
}

void Image::bright(int a) {
    uint16_t brightness;
    const uint16_t offset = a;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            brightness = data[i + j] + offset;
            data[i + j] = brightness > 255 ? 255 : (brightness < 0 ? 0 : (uint8_t) brightness);
        }
    }
}

void Image::contrast(float a) {
    if (a < 0) return;
    float brightness;
    const float offset = a;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            brightness = (float) data[i + j] * offset;
            data[i + j] = brightness > 255 ? 255 : (uint8_t) brightness;
        }
    }
}

Image Image::convolve(const float *kernel, uint8_t r_, uint8_t c_) {
    float sum;
    Image cpy = clone();
    uint8_t l = r_ / 2, m = c_ / 2;
    for (int i = l; i < rows - l; ++i) {
        for (int j = m; j < cols - m; ++j) {
            sum = 0;
            for (int row = -l; row <= l; ++row) {
                for (int col = -m; col <= m; ++col) {
                    sum += (float) get_(i + row, j + col) * kernel[(row + l) * c_ + col + m];
                }
            }
            sum = (sum + 0.5) > 255 ? 255 : sum + (float) 0.5;
            cpy.set_(i, j, (uint8_t) sum);
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

uint8_t Image::get_(int row, int col) {
    return data[(row * cols + col) * channels];
}

uint8_t *Image::get(int row, int col) {
    auto pixels = static_cast<uint8_t *>(init("uint8_t", channels));
    int offset = (row * cols + col) * channels;
    for (int i = 0; i < channels; ++i) {
        pixels[i] = data[offset + i];
    }
    return pixels;
}

void Image::set_(int row, int col, uint8_t value) {
    data[row * cols + col] = value;
}

void Image::set(int row, int col, const uint8_t *values) {
    if (values == nullptr) return;
    int offset = (row * cols + col) * channels;
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
    uint16_t value;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            value = data[i + j] + (uint16_t) rhs.data[i + j];
            data[i + j] = value > 255 ? 255 : (uint8_t) value;
        }
    }
}

void Image::mul(const Image &rhs) {
    uint64_t value;
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; ++j) {
            value = data[i + j] * (uint64_t) rhs.data[i + j];
            data[i + j] = value > 255 ? 255 : (uint8_t) value;
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
    os << "Rows: " << image.rows << " Columns: " << image.cols << " Channels: " << image.channels ;
    return os;
}

Image Image::gray() {
    return toGray();
}

Image Image::brightness() {
    return toGray();
}

Image Image::luminance(float r, float g, float b) {
    if (new_channel < 3) { return toGray(); }
    float new_pixel_value;
    Image _gray(rows, cols, channels);
    for (int i = 0; i < size; i += channels) {
        new_pixel_value = r * data[i];
        new_pixel_value = new_pixel_value > 255 ? 255 : (new_pixel_value < 0 ? 0 : new_pixel_value);
        _gray.data[i] = (uint8_t) new_pixel_value;
        new_pixel_value = g * data[i + 1];
        new_pixel_value = new_pixel_value > 255 ? 255 : (new_pixel_value < 0 ? 0 : new_pixel_value);
        _gray.data[i + 1] = (uint8_t) new_pixel_value;
        new_pixel_value = b * data[i + 2];
        new_pixel_value = new_pixel_value > 255 ? 255 : (new_pixel_value < 0 ? 0 : new_pixel_value);
        _gray.data[i + 2] = (uint8_t) new_pixel_value;
        if (new_channel < channels) {
            _gray.data[i + 3] = data[i + 3];
        }
    }
    _gray.write("data/test.png");
    printf("Luminance\n %d %d %d %d\n", new_channel == channels, new_channel, channels, &_gray == this);
    return _gray;
}

Image Image::clamping(uint8_t low, uint8_t high) {
    int pixel_value;
    Image _gray(rows, cols, channels);
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; j++) {
            pixel_value = data[i + j];
            _gray.data[i + j] = pixel_value > high ? high : (pixel_value < low ? low : pixel_value);
        }
        if (new_channel < channels) _gray.data[i + new_channel] = data[i + new_channel];
    }
    return _gray;
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
    Image invert_(rows, cols, channels);
    for (int i = 0; i < size; i += channels) {
        for (int j = 0; j < new_channel; j++) {
            invert_.data[i + j] = _max - data[i + j];
        }
        if (new_channel < channels) invert_.data[i + new_channel] = data[i + new_channel];
    }

    return invert_;
}

void Image::gamma() {

}

void Image::Sobel() {

}

void Image::Gauss() {

}

Image Image::add(const Image &a, const Image &b) {
    Image result(a.rows, a.cols, a.channels);
    result += a;
    result += b;
    return result;
}




