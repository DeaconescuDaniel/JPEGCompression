#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "testHelpers.cpp"
#include "rgbToYCC.h"

using namespace cv;

TEST(RGBToYCCTest, SolidColor) {
    Mat blueImage(16, 16, CV_8UC3, Scalar(255, 0, 0));
    Mat yccImage = RGBtoYCC(blueImage);
    Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual(blueImage, recoveredImage));
}

TEST(RGBToYCCTest, Grayscale) {
    Mat grayImage(16, 16, CV_8UC3, Scalar(100, 100, 100));
    Mat yccImage = RGBtoYCC(grayImage);

    std::vector<Mat> channels;
    split(yccImage, channels);
    EXPECT_EQ(countNonZero(channels[1]), 0);
    EXPECT_EQ(countNonZero(channels[2]), 0);
}

TEST(RGBToYCCTest, RandomImage) {
    Mat randomImage(16, 16, CV_8UC3);
    randu(randomImage, Scalar(0, 0, 0), Scalar(255, 255, 255));

    Mat yccImage = RGBtoYCC(randomImage);
    Mat recoveredImage = YCCtoRGB(yccImage);

    EXPECT_TRUE(areImagesEqual(randomImage, recoveredImage, 2.0));
}

