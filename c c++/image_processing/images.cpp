//
// Created by Thibaut Temkeng on 08.08.2021.
//

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

#include "images.h"
#include "utils.h"

Image::~Image()
{
    free(data);
}

Image::Image(const char *filename)
{
    if (read(filename))
    {
        printf("File %s is read\n", filename);
        size = channels * rows * cols;
        new_channel = channels > 3 ? 3 : channels;
        min_max();
    }
    else
        printf("File %s was not read\n", filename);
}

Image::Image(int n_rows, int n_cols, int n_channels) : rows(n_rows), cols(n_cols), channels(n_channels)
{
    size = n_channels * n_rows * n_cols;
    new_channel = channels > 3 ? 3 : channels;
    float init_value = 0;
    this->data = init(this->size, init_value);
}

Image::Image(const Image &image) : rows(image.rows), cols(image.cols), channels(image.channels), size(image.size),
                                   new_channel(image.new_channel), _min(image._min), _max(image._max)
{

    this->data = copy(image.data, size);
}

bool Image::read(const char *filename)
{
    uint8_t *tmp = stbi_load(filename, &cols, &rows, &channels, 0);
    size = cols * rows * channels;
    float none(0.0);
    data = cast(tmp, size, none);
    return data != nullptr;
}

bool Image::write(const char *filename)
{
    int check;
    float *cpy = static_cast<float *>(copy(this->data, size));

    if (_max > 255 or _min < 0)
    {
        normalize(cpy, rows * cols * channels);
    }
    uint8_t *_data = cast(cpy, size, new_channel);
    switch (getType(filename))
    {
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

Image::Type Image::getType(const char *filename)
{
    const char *extension = strrchr(filename, '.');
    if (extension == nullptr)
        return PNG;
    if (strcmp(extension, ".jpg") == 0 or strcmp(extension, ".jpeg") == 0)
        return JPG;
    else if (strcmp(extension, ".bmp") == 0)
        return BMP;
    else if (strcmp(extension, ".tga") == 0)
        return TGA;
    return PNG;
}

/**
 *
 * @return  The image data.
 */
float *Image::getData() const
{
    return data;
}

int Image::rows_() const
{
    return rows;
}

int Image::height() const
{
    return rows;
}

int Image::cols_() const
{
    return cols;
}

int Image::width() const
{
    return cols;
}

int Image::getChannels() const
{
    return channels;
}

Image Image::toGray()
{
    if (grayscale == nullptr)
    {
        if (new_channel == 1)
        {
            grayscale = new Image(*this);
            return *grayscale;
        }
        int index = 0;
        float pixelSomme;
        grayscale = new Image(rows, cols, 1);
        for (int i = 0; i < size; i += channels)
        {
            pixelSomme = 0;
            for (int j = 0; j < new_channel; j++)
            {
                pixelSomme += data[i + j];
            }
            grayscale->setData(index++, pixelSomme / (float)new_channel);
        }
        grayscale->min_max();
    }
    //    grayscale->_min = getMin(grayscale->data, grayscale->size);
    //    grayscale->_max = getMax(grayscale->data, grayscale->size);
    return *grayscale;
}

Image Image::gray()
{
    return toGray();
}

Image Image::brightness()
{
    return toGray();
}
Histogram Image::getHistogram()
{
    //    DEBUG("Histogram", "getHistogram")
    if (histogram == nullptr)
    {
        uint8_t none(0);
        uint8_t *tmp_data = cast(this->data, this->size, none);
        histogram = new Histogram(rows, cols, channels, tmp_data);
    }
    return *histogram;
}

void Image::setData(int index, float value)
{
    data[index] = value;
    if (value > _max)
        _max = value;
    if (value < _min)
        _min = value;
}

void Image::setData(float *data_, size_t length)
{
    for (int i = 0; i < length; ++i)
    {
        setData(i, data_[i]);
    }
}

void Image::printData(const size_t length) const
{
    for (int i = 0; i < length; i++)
    {
        printf("%.f ", data[i]);
    }
    printf("\n");
}

float Image::min()
{
    if (first_min_max)
        min_max();
    return _min;
}

float Image::max()
{
    if (first_min_max)
        min_max();
    return _max;
}

void Image::min_max()
{
    _min = data[0], _max = data[0];
    for (int i = 1; i < size; ++i)
    {
        if (data[i] > _max)
            _max = data[i];
        if (data[i] < _min)
            _min = data[i];
    }
    first_min_max = false;
}

int Image::size_() const
{
    return size;
}

Image Image::autoContrast(float a_min, float a_max)
{
    Image cpy = clone();
    float dist = a_max - a_min;
    float dist_ = _max - _min;
    if (dist == 0 or dist_ == 0)
        return cpy;
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            cpy.data[i + j] = a_min + (data[i + j] - _min) * dist / dist_;
        }
    }
    cpy._min = a_min;
    cpy._max = a_max;
    return cpy;
}

Image Image::equalize()
{
    //    DEBUG(result, "EQUALIZE")
    float mn = (float)size;
    Histogram hist = getHistogram();
    Image result = clone();
    for (int i = 0; i < size; i += new_channel)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            result.data[i + j] = ((float)hist.cumsum[(uint8_t)data[i + j]] * _max) / mn;
        }
    }
    return result;
}

void Image::bright(int a)
{
    const float offset = a;
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            data[i + j] += offset;
        }
    }
}

Image Image::contrast(float alpha)
{
    Image result = clone();
    if (alpha < 0)
        return result;
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            result.data[i + j] *= alpha;
        }
    }
    return result;
}

Image Image::convolve(const float *kernel, uint8_t r_, uint8_t c_)
{
    float sum;
    int index = 0;
    Image cpy = clone();
    //    auto *data_result = static_cast<float *>(init("float", size, 0, "float", cpy.data));
    uint8_t l = r_ / 2, m = c_ / 2;
    for (int color = 0; color < new_channel; ++color)
    {
        for (int i = l; i < rows - l; ++i)
        {
            for (int j = m; j < cols - m; ++j)
            {
                sum = 0;
                for (int row = -l; row <= l; ++row)
                {
                    for (int col = -m; col <= m; ++col)
                    {
                        sum += at(i + row, j + col, color) * kernel[(row + l) * c_ + col + m];
                    }
                }
                cpy.set_at(i, j, sum, color);
            }
        }
    }
    return cpy;
}

Image Image::clone()
{
    Image cpy(rows, cols, channels);
    cpy.data = copy(data, size);
    cpy._max = _max;
    cpy._min = _min;
    return cpy;
}

float Image::at(int row, int col, uint8_t color)
{
    if ((row < 0 or row >= rows) or (col < 0 or col >= cols))
        return INFINITY;
    return data[(row * cols + col) * channels + color];
}

void Image::set_at(int row, int col, float value, uint8_t color)
{
    if (value > _max)
        _max = value;
    if (value < _min)
        _min = value;
    data[(row * cols + col) * channels + color] = value;
}

float *Image::getPixel(int row, int col)
{
    float init_value = 0;
    float *pixels = init(channels, init_value);
    int offset = (row * cols + col) * channels;
    for (int i = 0; i < channels; ++i)
    {
        pixels[i] = data[offset + i];
    }
    return pixels;
}

void Image::setPixel(int row, int col, const float *values)
{
    if (values == nullptr)
        return;
    int offset = (row * cols + col) * channels;
    for (int i = 0; i < channels; ++i)
    {
        if (values[i] > _max)
            _max = values[i];
        if (values[i] < _min)
            _min = values[i];
        data[offset + i] = values[i];
    }
}

void Image::minus(const Image &rhs)
{
    _max = 0;
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            data[i + j] -= rhs.data[i + j];
            if (data[i + j] < _min)
                _min = data[i + j];
            if (data[i + j] > _max)
                _max = data[i + j];
        }
    }
}

void Image::add(const Image &rhs)
{
    _min = _max, _max = 0;
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            data[i + j] = data[i + j] + rhs.data[i + j];
            if (data[i + j] < _min)
                _min = data[i + j];
            if (data[i + j] > _max)
                _max = data[i + j];
        }
    }
}

void Image::mul(const Image &rhs)
{
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; ++j)
        {
            data[i + j] *= rhs.data[i + j];
            if (data[i + j] < _min)
                _min = data[i + j];
            if (data[i + j] > _max)
                _max = data[i + j];
        }
    }
}

Image Image::operator+(const Image &rhs) const
{
    Image plus(*this);
    plus.add(rhs);
    return plus;
}

Image Image::operator-(const Image &rhs) const
{
    Image sub(*this);
    sub.minus(rhs);
    return sub;
}

Image Image::operator*(const Image &rhs) const
{
    Image multiply(*this);
    multiply.mul(rhs);
    return multiply;
}

Image &Image::operator+=(const Image &rhs)
{
    add(rhs);
    return *this;
}

Image &Image::operator-=(const Image &rhs)
{
    minus(rhs);
    return *this;
}

Image &Image::operator*=(const Image &rhs)
{
    mul(rhs);
    return *this;
}

bool Image::operator==(const Image &rhs) const
{
    return data == rhs.data &&
           rows == rhs.rows &&
           cols == rhs.cols &&
           channels == rhs.channels;
}

bool Image::operator!=(const Image &rhs) const
{
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Image &image)
{
    os << "Rows: " << image.rows << " Columns: " << image.cols << " Channels: " << image.channels;
    return os;
}

Image Image::luminance(float r, float g, float b)
{
    if (new_channel < 3)
    {
        return toGray();
    }
    Image lum(rows, cols, channels);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            lum.set_at(i, j, at(i, j) * r);
            lum.set_at(i, j, at(i, j, 1) * g, 1);
            lum.set_at(i, j, at(i, j, 2) * b, 2);
            for (int k = new_channel; k < channels; ++k)
            { // For png photos
                lum.set_at(i, j, at(i, j, k), k);
            }
        }
    }
    return lum;
}

Image Image::clamping(float low, float high)
{
    float pixel_value;
    Image clamp = clone();
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; j++)
        {
            pixel_value = data[i + j];
            clamp.setData(i + j, pixel_value > high ? high : (pixel_value < low ? low : pixel_value));
        }
    }
    return clamp;
}

Image Image::remove_n_last_col(int n)
{
    Image result(rows, cols - n, channels);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols - n; ++j)
        {
            result.setPixel(i, j, getPixel(i, j));
        }
    }
    return result;
}

Image Image::resize(int row, int col)
{

    Image result(row, col, channels);
    DEBUG(result, "RESIZE");
    perror("Resize does not work correctly");
    return result;
    uint8_t init_value = 0;
    uint8_t *data_out = init(result.size, init_value);
    uint8_t *data_original = cast(this->data, this->size, init_value);
    stbir_resize_uint8(data_original, width(), height(),
                       cols * channels * sizeof(uint8_t), data_out, result.width(),
                       result.height(), col * channels * sizeof(uint8_t), channels);
    float *data_result = cast(data_out, result.size, data[0]);
    memcpy(result.data, data_result, result.size);
    return result;
}

Image Image::threshold(float thresh)
{
    Image result(rows, cols, channels);
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; j++)
        {
            result.data[i + j] = data[i + j] > thresh ? 255 : 0;
        }
        if (new_channel < channels)
        {
            result.data[i + new_channel] = data[i + new_channel];
        }
    }
    result._max = 255;
    result._min = 0;
    return result;
}

Image Image::invert()
{
    Image invert_ = clone();
    for (int i = 0; i < size; i += channels)
    {
        for (int j = 0; j < new_channel; j++)
        {
            invert_.data[i + j] = _max - data[i + j];
        }
    }
    return invert_;
}

void Image::gamma()
{
}

Image Image::sobel()
{
    float u[] = {1, 2, 1};
    float v[] = {-1, 0, 1};
    float kernel_x[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    float kernel_y[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    Image *_sobel = new Image(add_padding(3, 3).blur(3));

    //    Image s_x = _blur->convolve(u, 3, 1).convolve(v, 1, 3);
    //    Image s_y = _blur->convolve(v, 3, 1).convolve(u, 1, 3);

    Image s_x = _sobel->convolve(kernel_x, 3, 3).remove_padding(3, 3);
    Image s_y = _sobel->convolve(kernel_y, 3, 3).remove_padding(3, 3);

    _sobel = new Image(rows, cols, channels);
    for (int i = 0; i < size; ++i)
    {
        _sobel->data[i] = std::sqrt(s_x.data[i] * s_x.data[i] + s_y.data[i] * s_y.data[i]);
    }
    _sobel = new Image(_sobel->brightness());
    normalize(_sobel->data, _sobel->size);
    return *_sobel;
}

Image Image::gauss(int radius, float sigma)
{
    int i = 0, j = 0, length = radius * radius;
    float value = 0;
    float *kernel = init(length, value);
    sigma = sigma == 0.0 ? float(0.3 * ((radius - 1) * 0.5 - 1) + 0.8) : sigma;
    sigma *= 2 * sigma;
    for (int x = -radius / 2; x <= 0; ++x)
    {
        j = 0;
        for (int y = -radius / 2; y <= 0; ++y)
        {
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
    for (i = 0; i < length; ++i)
    {
        value += kernel[i];
    }
    //    Normalize the kernel
    for (i = 0; i < length; ++i)
    {
        kernel[i] /= value;
    }
    //    print(kernel, radius, radius);
    return convolve(kernel, radius, radius);
}

Image Image::add(const Image &a, const Image &b)
{
    Image result(a);
    result += b;
    return result;
}

Image Image::minus(const Image &a, const Image &b)
{
    Image result(a);
    result -= b;
    return result;
}

Image Image::convolve(Image &image, const float *kernel, uint8_t r, uint8_t c)
{
    return image.convolve(kernel, r, c);
}

void Image::norm(float low, float high)
{
    float *cpy = copy(this->data, this->size);
    normalize(cpy, size, low, high);
    setData(cpy, size);
}

template <typename T>
void Image::normalize(T *_data, int size, float low, float high)
{
    T max_ = _data[0], min_ = _data[0];
    for (int i = 1; i < size; ++i)
    {
        if (_data[i] > max_)
            max_ = _data[i];
        if (_data[i] < min_)
            min_ = _data[i];
    }
    T dist = max_ - min_;
    T high_low = high - low;
    if (dist == 0)
        return;
    for (int i = 0; i < size; i++)
    {
        _data[i] = high_low * (_data[i] - min_) / dist;
    }
}

Image Image::add_padding(int _row, int _col)
{
    Image zp(rows + 2 * _row, cols + 2 * _col, channels);
    int index, offset;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            index = (i * cols + j) * channels;
            offset = ((i + _row) * zp.cols + j + _col) * channels;
            for (int k = 0; k < channels; ++k)
            {
                zp.data[offset + k] = data[index + k];
            }
        }
    }
    return zp;
}

Image Image::remove_padding(int _row, int _col)
{
    Image zp(rows - 2 * _row, cols - 2 * _col, channels);
    int index, offset;
    for (int i = _row; i < rows - _row; ++i)
    {
        for (int j = _col; j < cols - _col; ++j)
        {
            index = (i * cols + j) * channels;
            offset = ((i - _row) * zp.cols + j - _col) * channels;
            for (int k = 0; k < channels; ++k)
            {
                zp.data[offset + k] = data[index + k];
            }
        }
    }
    return zp;
}

Image Image::blur(int kernel_size)
{
    float *kernel = init(kernel_size * kernel_size, float(1) / float(kernel_size * kernel_size));

    return clone().convolve(kernel, kernel_size, kernel_size);
}

template <typename T>
int getMinPos(T *array, size_t length)
{
    int pos = 0;
    T min_value = array[0];
    for (int i = 1; i < length; ++i)
    {
        if (array[i] < min_value)
        {
            min_value = array[i];
            pos = i;
        }
    }
    return pos;
}

template <typename T>
int get_max_pos(T *array, size_t length)
{
    int pos = 0;
    T max_value = array[0];
    for (int i = 1; i < length; ++i)
    {
        if (array[i] > max_value)
        {
            max_value = array[i];
            pos = i;
        }
    }
    return pos;
}

Image Image::shrink_n(int n)
{
    float max_value = _max + 1;
    Image *image = new Image(clone());
    Image *energy = new Image(sobel());
    Image energy_map = energy->clone();
    Image positions(energy->rows_(), energy->cols_(), 1);
    int pos, pos_, *path = init(image->height(), 0);
    float lur[3], white[3] = {max_value, max_value, max_value}, energy_max_value = energy->_max + 1;
    //    DEBUG(this->energy->size_(), "FIND_SEAM_")
    for (int k = 0; k < n; ++k)
    {
        for (int i = energy->rows_() - 2; i > -1; --i)
        {
            for (int j = 0; j < energy->cols_() - k; ++j)
            {
                lur[0] = energy->at(i + 1, j - 1);
                lur[1] = energy->at(i + 1, j);
                lur[2] = energy->at(i + 1, j + 1);
                pos = getMinPos(lur, 3);
                energy_map.set_at(i, j, lur[pos] + energy->at(i, j));
                positions.set_at(i, j, (float)pos);
            }
        }
        pos_ = getMinPos(energy_map.getData(), energy->cols_() - k);
        for (int i = 0; i < energy->rows_(); ++i)
        {
            path[i] = pos_;
            //            Roll the row
            for (int j = pos_; j < energy->cols_() - 1 - k; ++j)
            {
                image->setPixel(i, j, image->getPixel(i, j + 1));
                energy->set_at(i, j, energy->at(i, j + 1));
            }
            image->setPixel(i, energy->cols_() - 1 - k, white);
            energy->set_at(i, energy->cols_() - 1 - k, energy_max_value);
            pos_ = pos_ - 1 + (int)positions.at(i, pos_);
        }
    }
    //    Resize the image and the energy
    //    energy = new Image(energy->remove_n_last_col(n));
    //    image = new Image(image->remove_n_last_col(n));
    free(energy);
    return image->remove_n_last_col(n);
}

void Image::show_seam()
{
    int pos;
    float lur[3];
    Image energy(sobel());
    float seam_color[] = {255, 0, 0};
    int *path = new int[height()];
    Image positions(rows_(), cols_(), getChannels());
    for (int i = rows_() - 2; i > -1; --i)
    {
        for (int j = 0; j < cols_(); ++j)
        {
            lur[0] = energy.at(i + 1, j - 1);
            lur[1] = energy.at(i + 1, j);
            lur[2] = energy.at(i + 1, j + 1);
            pos = getMinPos(lur, 3);
            energy.set_at(i, j, lur[pos] + energy.at(i, j));
            positions.set_at(i, j, (float)pos);
        }
    }
    int pos_ = getMinPos(energy.getData(), energy.cols_());
    //    DEBUG(energy, "SHOW_SEAM")
    for (int i = 0; i < energy.rows_(); ++i)
    {
        path[i] = pos_;
        energy.set_at(i, pos_, 0);
        setPixel(i, pos_, seam_color);
        pos_ = pos_ - 1 + (int)positions.at(i, pos_);
    }
}

Image Image::flip_v()
{
    Image flip(rows, cols, channels);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; j++)
        {
            flip.setPixel(i, cols - 1 - j, getPixel(i, j));
        }
    }
    return flip;
}

Image Image::flip_h()
{
    Image flip(rows, cols, channels);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; j++)
        {
            flip.setPixel(rows - 1 - i, j, getPixel(i, j));
        }
    }
    return flip;
}

Image Image::rot180()
{
    Image rot_180(rows, cols, channels);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; j++)
        {
            rot_180.setPixel(rows - 1 - i, cols - 1 - j, getPixel(i, j));
        }
    }
    return rot_180;
}

Image Image::rot90()
{
    Image rot_90(cols, rows, channels);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; j++)
        {
            rot_90.setPixel(j, i, getPixel(i, j));
        }
    }
    return rot_90;
}

Image Image::concat_v(Image &image)
{
    if (cols != image.cols)
    {
        DEBUG("The image must have the same number of columns.", "Concat_v")
        return nullptr;
    }
    Image concatenateImage(rows + image.rows, cols, channels);
    //    DEBUG(concatenateImage, "Concat_v")
    //    DEBUG(*this, "Concat_v")
    //    DEBUG(image, "Concat_v")
    for (int i = 0; i < concatenateImage.rows; ++i)
    {
        for (int j = 0; j < cols; j++)
        {
            i < rows ? concatenateImage.setPixel(i, j, getPixel(i, j)) : concatenateImage.setPixel(i, j, image.getPixel(i - rows, j));
        }
    }

    return concatenateImage;
}

Image Image::concat_h(Image &image)
{
    if (rows != image.rows)
    {
        DEBUG("The image must have the same number of rows.", "Concat_h")
        return nullptr;
    }
    Image concatenateImage(rows, cols + image.cols, channels);
    //    DEBUG(concatenateImage, "Concat_h")
    //    DEBUG(*this, "Concat_h")
    //    DEBUG(image, "Concat_h")
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < concatenateImage.cols; j++)
        {
            j < cols ? concatenateImage.setPixel(i, j, getPixel(i, j)) : concatenateImage.setPixel(i, j, image.getPixel(i, j - cols));
        }
    }

    return concatenateImage;
}

Tree *new_tree(float initial_value)
{
    Tree *m = static_cast<Tree *>(malloc(sizeof(Tree)));
    if (m == nullptr)
        return nullptr;
    m->left = nullptr;
    m->right = nullptr;
    m->l = nullptr;
    m->r = nullptr;
    m->u = nullptr;
    m->value = initial_value;
    return m;
}
// TREE

Tree::Tree() : _node()
{
    this->left = nullptr;
    this->right = nullptr;
    this->next = nullptr;
    this->l = nullptr;
    this->r = nullptr;
    this->u = nullptr;
    this->value = 0;
}

Tree::Tree(float value) : Tree()
{
    this->value = value;
}

Tree::Tree(Image image) : _node()
{
    Tree *trees = new Tree[image.size_()];
    float *data = image.getData();
    int col = 0, row = 0;
    for (int i = 0; i < image.rows_(); i++)
    {
        for (int j = 0; j < image.cols_(); j++)
        {
            trees[row + j].value = image.at(i, j);
            if (j != 0)
            {
                trees[row + j].left = &trees[row + j - 1];
                if (i != image.rows_() - 1)
                {
                    trees[row + j].l = &trees[row + image.cols_() + j - 1];
                }
            }
            if (j + 1 != image.cols_())
            {
                trees[row + j].right = &trees[row + j + 1];
                if (i != image.rows_() - 1)
                {
                    trees[row + j].r = &trees[row + image.cols_() + j + 1];
                }
            }
            if (i != image.rows_() - 1)
            {
                trees[row + j].u = &trees[row + image.cols_() + j];
            }
            trees[row + j].next = trees[row + j].value == 0 ? trees[row + j].l : (trees[row + j].value == 1 ? trees[row + j].u : trees[row + j].r);
        }
        row += image.cols_();
    }

    //    image.printData(10);
    //    for (int i = 0; i < 10; i++) {
    //        std::cout << trees[i];
    //    }
    //    col = 20;
    //    for (int i = 0; i < col; i++) {
    //        for (int j = 0; j < 2*col; ++j)
    //            std::cout << image.at(i, j) << " ";
    //        printf("\n");
    //    }
    //    printf("\n");
}

std::ostream &operator<<(std::ostream &os, const Tree &tree)
{
    os << "L: " << ((tree.left == nullptr) ? INFINITY : tree.left->value)
       << " V: " << tree.value
       << " R: " << ((tree.right == nullptr) ? INFINITY : tree.right->value)
       << " N: " << ((tree.next == nullptr) ? INFINITY : tree.next->value)
       << " BL: " << ((tree.l == nullptr) ? INFINITY : tree.l->value)
       << " BU: " << ((tree.u == nullptr) ? INFINITY : tree.u->value)
       << " BR: " << ((tree.r == nullptr) ? INFINITY : tree.r->value)
       << std::endl;
    return os;
}
