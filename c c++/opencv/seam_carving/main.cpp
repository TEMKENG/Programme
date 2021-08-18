#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    string filename = "data/g-star-shoe.jpg";
//    Mat img = imread(filename.c_str());
    Mat img = imread("data/g-star-shoe.jpg");
//    cv::Mat img = cv::imread(R"(D:\Programme\c c++\opencv\seam_carving\data\g-star-shoe.jpg)");
    // Error Handling
    if (img.empty()) {
        cout << "Image File "
             << "Not Found" << endl;

        // wait for any key press
        cin.get();
        return -1;
    }
    std::cout << "Hello, World!" << std::endl;
    imshow("Image", img);
    waitKey(0);
    return 0;
}
