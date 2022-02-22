//
// Created by Thibaut Temkeng on 16.11.2021.
//

#ifndef IMAGE_PROCESSING_UTILS_H
#define IMAGE_PROCESSING_UTILS_H

using namespace std;

#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#define MAX 256
#define SEP " "
#define COL 200
#define ROW 50

#define DEBUG(x, prefix) std::cout << (strcmp(prefix, "") == 0 ? "Images " : prefix) << "::" << #x << " = (" << x << ")" << std::endl;

template <typename T, typename Dtype>
Dtype *cast(T *src, size_t length, Dtype none);

template <typename T>
T *copy(T *pointer, size_t length);

template <typename T>
T getMax(T *data, size_t size);

template <typename T>
T getMin(T *data, size_t size);

/**
 * Initializes the pointer `buffer` of length `length` with `Value`.
 *
 * @param buffer The pointer to be initialized.
 * @param length The length of buffer.
 * @param value The length of buffer.
 */

template <typename T>
void init(T *buffer, size_t length, T value);

/**
 * Creates a pointer of length `length` and init it with `value`.
 *
 * @param length The length of buffer.
 * @param value The length of buffer.
 *
 * @return A pointer of type T
 *
 */
template <typename T>
T *init(size_t length, T value);



/**
 * Displays data in command line.\n
 * The data can be printed on one line or on several lines.
 * @param data Data to be displayed.
 * @param col The number of items to be printed on a line.
 * @param row The number of line.
 */
template <typename T>
void print(const T *data, int col, int row);


template <typename T, typename Dtype>
Dtype *cast(T *src, size_t length, Dtype none)
{
    Dtype *dst = new Dtype[length];
    for (int i = 0; i < length; i++)
    {
        dst[i] = (Dtype) src[i];
    }
    return dst;
}


template <typename T>
T *copy(T *pointer, size_t length)
{
    T *buffer = new T[length];
    for (int i = 0; i < length; i++)
        buffer[i] = pointer[i];

    return buffer;
}

template <typename T>
T getMax(T *data, size_t size)
{
    T max_value = data[0];
    for (int i = 1; i < size; i++)
    {
        if (data[i] > max_value)
            max_value = data[i];
    }
    return max_value;
}

template <typename T>
T getMin(T *data, size_t size)
{
    T min_ = data[0];
    for (int i = 1; i < size; i++)
    {
        if (data[i] < min_)
            min_ = data[i];
    }
    return min_;
}

template <typename T>
void init(T *buffer, size_t length, T default_value)
{
    for (int i = 0; i < length; ++i)
        buffer[i] = default_value;
}

template <typename T>
T *init(size_t length, T value)
{
    T *buffer = new T[length];
    for (int i = 0; i < length; ++i)
        buffer[i] = value;

    return buffer;
}

template <typename T>
void print(const T *data, int col, int row)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            cout << data[i * row + j] << " ";
        printf("\n");
    }
}
#endif // IMAGE_PROCESSING_UTILS_H
