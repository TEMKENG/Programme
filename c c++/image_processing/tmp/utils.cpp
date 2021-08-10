//
// Created by TEMKENG on 06.08.2021.
//
#include "utils.h"


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
