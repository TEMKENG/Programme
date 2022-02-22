//
// Created by TEMKENG on 18.11.2021.
//

#include "utils.h"

template<typename T>
void copy_(T *dst, const T *src, size_t length) {
    for (int i = 0; i < length; ++i) {
        dst[i] = src[i];
    }
}

template<typename T>
T getMax(T *data, size_t size) {
    T max_value = data[0];
    for (auto i = 1; i < size; i++) {
        if (data[i] > max_value)
            max_value = data[i];
    }
    return max_value;
}

template<typename T>
T getMin(T *data, size_t size) {
    T min_ = data[0];
    for (auto i = 1; i < size; i++) {
        if (data[i] < min_)
            min_ = data[i];
    }
    return min_;
}

template<typename T>
void init(T *buffer, size_t length) {
    buffer = new T[length];
    for (int i = 0; i < length; ++i) {
        buffer[i] = 0;
    }
}

void *init(const char *dst_type, size_t length, double initial_value, const char *src_type, void *pointer) {

    if (strcmp(dst_type, "int") == 0) {
        int *buffer = new int[length];
        if (strcmp(src_type, "int") == 0) {
            int *cpy = static_cast<int *>(pointer);
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
            float *cpy = static_cast<float *>(pointer);
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
            uint16_t *cpy = static_cast<uint16_t *>(pointer);
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
            float *cpy = static_cast<float *>(pointer);
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
            double *cpy = static_cast<double *>(pointer);
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

void print(const int *data, int col, int row) {
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

void print(const uint8_t *data, int col, int row) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d ", data[i * row + j]);
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