#include "images.h"
#include <iostream>


int main() {
    std::cout << "IMAGE PROCESSING!" << std::endl;
    clock_t t = clock();
    srand(time(nullptr));
//    Image img("data/Alida.jpg");
//    Image img("data/pp.jpg");
//    Image img("data/profile.jpg");
//    Image img("data/screenshot.png");
//    Image img("data/g-star-shoe.jpg");
    Image img("data/image.jpg");

    img.brightness().sobel().write("data/sobel.png");
    Image zp = img.add_padding(10, 10);
    zp.write("data/padding_add.png");
    zp.remove_padding(10, 10).write("data/padding_remove.png");
//    Image img(500, 500, 3);

    float kernel[] = {1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9};
//    float kernel[] = {0.075, 0.125, 0.075, 0.125, 0.200, 0.125, 0.075, 0.125, 0.075};
//    float kernel[] = {-1, -2, 0, -2, 0, 2, 0, 2, 1};
//    float kernel[] = {1, 2, 1, 2, 3, 2, 1, 2, 1};

    std::cout << img << std::endl;
    auto values = static_cast<float *>(init("uint8_t", img.getChannels()));
    for (int i = 0; i < img.rows_(); i++) {
        for (int j = 0; j < img.getChannels(); j++) {
//            values[j] = rand() % 256;
//            values[j]  = 255;
        }
//        img.set(i, i, values);
//        img.set(i, img.cols_() - i, values);

    }
//    img.write("data/zeros.png");
//    Image gray = img.toGray();
//
//    gray.write("data/gray.png");
//    gray.autoContrast();
//    gray.write("data/gray_autocontrast.png");
//    Image equalize = img.equalize();
//    equalize.write("data/equalize.png");

    Image cpy = img.copy();

    for (int i = 0; i < img.rows_(); i++) {
        for (int j = 0; j < img.getChannels(); j++) {
            values[j] = rand() % 256;
            //            values[j]  = 255;
        }
        int index = rand() % cpy.cols_();
        cpy.set(i, index, values);
        cpy.set(i, cpy.cols_() - index, values);

    }
//    Image gray_ = cpy.toGray();
//    cpy.write("data/copy.png");
//    gray_.write("data/copy_gray_.png");

//    Image blur = gray.convolve(kernel, 3, 3);
//    blur.write("data/blur.png");
//    Image convolve1 = blur.convolve(kernel_, 3, 1);
//    convolve1.write("data/convolve_y.png");
//    Image convolve2 = blur.convolve(kernel_, 1, 3);
//    convolve2.write("data/convolve_x.png");
//
//    //    Image add_test = Image::add(convolve2, convolve1);
//    Image add_test = Image::add(convolve2, convolve1);
//    add_test.write("data/convolve_add.png");
//
//
//    Image convolve = convolve1 + convolve2;
//    convolve.write("data/convolve.png");
//
//    Image clamping = img.clamping(10, 200);
//    clamping.write("data/clamping.png");
//
//    Image thresh = img.gray().threshold(100);
//    thresh.write("data/threshold.png");
//
//    Image invert = img.invert();
//    invert.write("data/invert.png");
//    img.contrast(1.5);
//    img.write("data/contrast.png");
//    convolve1.add(convolve2);
//    convolve1 -= convolve2;

    double time_taken = ((double) (clock() - t)) / CLOCKS_PER_SEC; // Calculate the elapsed time
    printf("\nExecution time %.6f second\n", time_taken);
    std::cout << img << std::endl;
    Image luminance = img.luminance(1.0, 0.0, 0.0);
    printf("OKAY\n");
    luminance.write("data/luminance.png");

    return 0;
}
