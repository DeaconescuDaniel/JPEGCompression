#include <opencv2/opencv.hpp>

using namespace cv;

inline bool areImagesEqual(const cv::Mat& img1, const cv::Mat& img2, double epsilon = 1.0) {
    Mat diff;
    absdiff(img1, img2, diff);
    return cv::countNonZero(diff > epsilon) == 0;
}