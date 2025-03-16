#include <opencv2/opencv.hpp>

using namespace cv;

inline Mat generateRandomMatrix(int rows, int cols, int type) {
    Mat mat(rows, cols, type);
    randu(mat, Scalar(-128), Scalar(127));
    return mat;
}

inline void printImage(const Mat& img){
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (img.channels() == 1) {
                std::cout << static_cast<int>(img.at<float>(i, j)) << " ";
            } else if (img.channels() == 3) {
                Vec3b pixel = img.at<Vec3b>(i, j);
                std::cout << "("
                          << static_cast<int>(pixel[0]) << ","
                          << static_cast<int>(pixel[1]) << ","
                          << static_cast<int>(pixel[2]) << ") ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

inline bool areImagesEqual(const Mat& img1, const Mat& img2, double epsilon = 1.0) {
    assert(img1.size() == img2.size() && img1.type() == img2.type());

    Mat diff;
    absdiff(img1, img2, diff);

    if (diff.channels() != 1) {
        std::vector<Mat> channels;
        split(diff,channels);
        for(const auto & channel : channels)
        {
            if(countNonZero(channel > epsilon) != 0)
            {
                std::cout << "Difference not within tolerance!" << std::endl;
                printImage(channel);
                return false;
            }
        }
        return true;
    }

    return countNonZero(diff > epsilon) == 0;
}