#include "images.h"
#include <iostream>

int main() {
    std::cout << "IMAGE PROCESSING!" << std::endl;
    clock_t t = clock();
    srand(time(nullptr));
//    Image img("data/Alida.jpg");
//    Image img("data/pp.jpg");
//    Image img("data/screenshot.png");
//    Image img("data/profile.jpg");
    Image img("data/g-star-shoe.jpg");
//    Image img(500, 500, 3);
    Tree *tree = new_tree();
    Tree tmp[10];
    tree->value = 5;
    DEBUG(tree->value);
    DEBUG(tmp[0].value);
    Image positions = img.seam_carving();
    printf("okay\n");
    Tree tmp2 = Tree(positions);

    Image gray = img.brightness();
    Image cont = img.contrast(1.5);
    cont.write("data/contra.png");
    gray.getHistogram().plot();
    gray.write("data/gray.png");
    img.sobel().write("data/sobel.png");
    gray.gauss(3).write("data/gaussian_blur.png");
//
    Image zp = img.add_padding(10, 10);
    zp.write("data/padding_add.png");
    zp.remove_padding(10, 10).write("data/padding_remove.png");


//    float kernel[] = {0.075, 0.125, 0.075, 0.125, 0.200, 0.125, 0.075, 0.125, 0.075};
//    float kernel[] = {-1, -2, 0, -2, 0, 2, 0, 2, 1};
    float kernel_[] = {1, -2, 1};

    std::cout << img << std::endl;
    auto values = static_cast<float *>(init("float", img.getChannels()));
    for (int i = 0; i < img.rows_(); i++) {
        for (int j = 0; j < img.getChannels(); j++) {
//            values[j] = rand() % 256;
//            values[j]  = 255;
        }
//        img.set(i, i, values);
//        img.set(i, img.cols_() - i, values);

    }
//    img.write("data/zeros.png");

//    img.autoContrast().write("data/autocontrast.png");
//    img.equalize().write("data/equalize.png");
//    Image cpy = img.copy();
//    for (int i = 0; i < img.rows_() / 2; i++) {
//        for (int k = 0; k < 2; ++k) {
//            for (int j = 0; j < img.getChannels(); j++) {
//                values[j] = (rand() % 256);
//                //            values[j]  = 255;
//            }
//            int index = rand() % cpy.cols_();
//            cpy.set(i, index, values);
//            cpy.set(i, cpy.cols_() - index - 1, values);
//            cpy.set(img.rows_() - i - 1, index, values);
//            cpy.set(img.rows_() - i - 1, cpy.cols_() - index - 1, values);
//        }
//    }
//    cpy.write("data/copy.png");
//
    Image blur = gray.blur();
    blur.write("data/blur.png");
    Image convolve1 = blur.convolve(kernel_, 3, 1);
    convolve1.write("data/convolve_y.png");
    Image convolve2 = blur.convolve(kernel_, 1, 3);
    convolve2.write("data/convolve_x.png");
//
//    //    Image add_test = Image::add(convolve2, convolve1);
//    Image add_test = Image::add(convolve2, convolve1);
//    add_test.write("data/convolve_add.png");
//
    Image convolve = convolve1 * convolve2;
    convolve.write("data/corner.png");
    img.invert().write("data/invert.png");
    img.contrast(0.5).write("data/contrast.png");
    img.threshold(100).write("data/threshold.png");
    img.clamping(20, 180).write("data/clamping.png");

    double time_taken = ((double) (clock() - t)) / CLOCKS_PER_SEC; // Calculate the elapsed time
    printf("\nExecution time %.6f second\n", time_taken);

//    Image l = img.luminance(1.0, 1.0, 1.10);
//    l.write("data/luminance.png");
    printf("Finish\n");

    return 0;
}
