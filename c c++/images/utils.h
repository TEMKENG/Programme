//
// Created by TEMKENG on 18.11.2021.
//

#ifndef IMAGES_UTILS_H
#define IMAGES_UTILS_H


#include <cmath>
#include <cstdio>
#include <cstring>
#include <ostream>
#define MAX 256
#define SEP " "
#define COL 200
#define ROW 50

#define DEBUG(x, prefix) std::cout << (strcmp(prefix, "") == 0 ? "Images" : prefix) << "| " << #x << " = (" << x << ")" << std::endl;

/**
 * Copy the values of a pointer to another pointer.
 *
 * @param dst The destination pointer.
 * @param src The source pointer.
 * @param length The length of the pointer.
 */
template <typename T>
void copy_(T *dst, const T *src, size_t length);

template <typename T>
T getMax(T *data, size_t size);

template <typename T>
T getMin(T *data, size_t size);

/**
 * Initializes the pointer `buffer` of length `length` with the value 0.
 *
 * @param buffer The pointer to be initialized.
 * @param length The length of buffer.
 */
template <typename T>
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
void *init(const char *dst_type, size_t length, double initial_value = 0, const char *src_type = "", void *pointer = nullptr);

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
void print(const double *data, int c = 20, int r = 1);


#endif //IMAGES_UTILS_H
