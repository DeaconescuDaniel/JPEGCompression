#include <opencv2/opencv.hpp>

using namespace cv;

inline Mat generateRandomMatrix(int rows, int cols, int type) {
    Mat mat(rows, cols, type);
    randu(mat, Scalar(-128), Scalar(127));
    return mat;
}

template <typename T>
inline void printImage(const Mat& img) {
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            if (img.channels() == 1) {
                std::cout << static_cast<int>(img.at<T>(i, j)) << " ";
            } else if (img.channels() == 3) {
                Vec<T, 3> pixel = img.at<Vec<T, 3>>(i, j);
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


template <typename T>
inline bool areImagesEqual(const Mat& img1, const Mat& img2, double epsilon = 1.0) {
    assert(img1.size() == img2.size() && img1.type() == img2.type());

    Mat diff;
    absdiff(img1, img2, diff);

    if (diff.channels() != 1) {
        std::vector<Mat> channels;
        split(diff, channels);
        for (const auto& channel : channels) {
            if (countNonZero(channel > epsilon) != 0) {
                std::cout << "Difference not within tolerance!" << std::endl;
                printImage<T>(channel);
                return false;
            }
        }
        return true;
    }

    if (countNonZero(diff > epsilon) != 0) {
        printImage<T>(diff);
    }

    return countNonZero(diff > epsilon) == 0;
}
