//
// Created by TEMKENG on 06.08.2021.
//

#ifndef IMAGE_PROCESSING_UTILS_H
#define IMAGE_PROCESSING_UTILS_H

#include <cstdint>
#include <cstdio>
#include <cstring>

void init(int *buffer, size_t length);

void *init(const char *type, size_t length, void *pointer = nullptr);

void init(uint8_t *values, size_t length);

void copy_(int *dst, const int *src, size_t length);

void copy_(uint8_t *dst, const uint8_t *src, size_t length);

void print(const int *data, int length);

void print(const float *data, int length);

void print(const uint8_t *data, int length);

#endif //IMAGE_PROCESSING_UTILS_H
