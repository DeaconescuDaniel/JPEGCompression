#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "testHelpers.cpp"
#include "rgbToYCC.h"

using namespace cv;

TEST(RGBToYCCTest, SolidColor) {
    Mat blueImage(16, 16, CV_8UC3, Scalar(255, 0, 0));
    Mat yccImage = RGBtoYCC(blueImage);
    Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual<uchar>(blueImage, recoveredImage));
}

TEST(RGBToYCCTest, Grayscale) {
    cv::Mat grayImage(16, 16, CV_8UC3, cv::Scalar(100, 100, 100));
    cv::Mat yccImage = RGBtoYCC(grayImage);

    std::vector<cv::Mat> channels;
    cv::split(yccImage, channels);

    double minVal, maxVal;

    minMaxLoc(channels[1], &minVal, &maxVal);
    EXPECT_NEAR(minVal, 128, 5);
    EXPECT_NEAR(maxVal, 128, 5);

    minMaxLoc(channels[2], &minVal, &maxVal);
    EXPECT_NEAR(minVal, 128, 5);
    EXPECT_NEAR(maxVal, 128, 5);
}

TEST(RGBToYCCTest, RandomImage) {
    Mat randomImage(16, 16, CV_8UC3);
    randu(randomImage, Scalar(0, 0, 0), Scalar(255, 255, 255));

    Mat yccImage = RGBtoYCC(randomImage);
    Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual<uchar>(randomImage, recoveredImage, 3.0));
}

TEST(RGBToYCCTest, PureWhite) {
    cv::Mat whiteImage(16, 16, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat yccImage = RGBtoYCC(whiteImage);
    cv::Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual<uchar>(whiteImage, recoveredImage, 2.0));
}

TEST(RGBToYCCTest, PureBlack) {
    cv::Mat blackImage(16, 16, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat yccImage = RGBtoYCC(blackImage);
    cv::Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual<uchar>(blackImage, recoveredImage, 2.0));
}

TEST(RGBToYCCTest, NoisyImage) {
    cv::Mat noisyImage(16, 16, CV_8UC3);
    randn(noisyImage, cv::Scalar(128, 128, 128), cv::Scalar(30, 30, 30));

    cv::Mat yccImage = RGBtoYCC(noisyImage);
    cv::Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual<uchar>(noisyImage, recoveredImage, 5.0));
}

TEST(RGBToYCCTest, NonDivisibleSizes) {
    cv::Mat nonDivImage(15, 23, CV_8UC3);
    randu(nonDivImage, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));

    cv::Mat yccImage = RGBtoYCC(nonDivImage);
    cv::Mat recoveredImage = YCCtoRGB(yccImage);    // ingrid<3

    EXPECT_TRUE(areImagesEqual<uchar>(nonDivImage, recoveredImage, 3.0));
}