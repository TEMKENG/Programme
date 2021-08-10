//
// Created by TEMKENG on 05.08.2021.
//

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

Image::~Image() {
    stbi_image_free(data);
}

Image::Image(const char *filename) {
    if (read(filename)) {
        printf("File %s is read\n", filename);
        size = channels * rows * cols;
        new_channel = channels > 3 ? 3 : channels;
    } else
        printf("File %s was not read\n", filename);
}

Image::Image(int _rows, int _cols, int _channels) : rows(_rows), cols(_cols), channels(_channels) {
    size = channels * rows * cols;
    new_channel = channels > 3 ? 3 : channels;
    data = static_cast<uint8_t *>(init("uint8_t", size));
}


Image::Image(const Image &image) : rows(image.rows), cols(image.cols), channels(image.channels) {
    size = channels * rows * cols;
    memcpy(data, image.data, size);
    new_channel = channels > 3 ? 3 : channels;
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
    Image gray(rows, cols, 1);
    for (int i = 0; i < rows * cols * channels; i += channels) {
        pixelSomme = 0;
        for (int j = 0; j < new_channel; j++) { pixelSomme += data[i + j]; }
        gray.setData(index++, pixelSomme / new_channel);
    }
    return gray;
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

Image &Image::operator+(const Image &rhs) {
    add(rhs);
    return *this;
}

Image &Image::operator-(const Image &rhs) {
    minus(rhs);
    return *this;
}

Image &Image::operator*(const Image &rhs) {
    mul(rhs);
    return *this;
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



