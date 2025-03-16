#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "ImageBlockMatrix.h"
#include "testHelpers.cpp"

using namespace cv;

TEST(ImageBlockMatrixTest, EvenSizedImageWithSubsampling) {
    Mat image(16, 16, CV_8UC3, Scalar(100, 150, 200));
    ImageBlockMatrix matrix(image, 2);

    EXPECT_EQ(matrix.getRows(), 2);
    EXPECT_EQ(matrix.getCols(), 2);

    for (int i = 0; i < matrix.getRows(); i++) {
        for (int j = 0; j < matrix.getCols(); j++) {
            ImageBlock block = matrix.getBlockAt(i, j);

            EXPECT_EQ(block.Y.rows, 8);
            EXPECT_EQ(block.Y.cols, 8);
            EXPECT_EQ(countNonZero(block.Y - 100), 0);

            EXPECT_EQ(block.Cb.rows, 4);
            EXPECT_EQ(block.Cb.cols, 4);
            EXPECT_EQ(countNonZero(block.Cb - 150), 0);

            EXPECT_EQ(block.Cr.rows, 4);
            EXPECT_EQ(block.Cr.cols, 4);
            EXPECT_EQ(countNonZero(block.Cr - 200), 0);
        }
    }
}

TEST(ImageBlockMatrixTest, NonDivisibleImageWithSubsampling) {
    Mat image(19, 21, CV_8UC3, Scalar(50, 100, 150));
    ImageBlockMatrix matrix(image, 2);

    EXPECT_EQ(matrix.getRows(), 3);
    EXPECT_EQ(matrix.getCols(), 3);

    for (int i = 0; i < matrix.getRows(); i++) {
        for (int j = 0; j < matrix.getCols(); j++) {
            ImageBlock block = matrix.getBlockAt(i, j);

            EXPECT_EQ(block.Y.rows, 8);
            EXPECT_EQ(block.Y.cols, 8);
            EXPECT_EQ(countNonZero(block.Y - 50), 0);

            EXPECT_EQ(block.Cb.rows, 4);
            EXPECT_EQ(block.Cb.cols, 4);
            EXPECT_EQ(countNonZero(block.Cb - 100), 0);

            EXPECT_EQ(block.Cr.rows, 4);
            EXPECT_EQ(block.Cr.cols, 4);
            EXPECT_EQ(countNonZero(block.Cr - 150), 0);
        }
    }
}

TEST(ImageBlockMatrixTest, OddSizedImageWithSubsampling) {
    Mat image(17, 19, CV_8UC3, Scalar(30, 60, 90));
    ImageBlockMatrix matrix(image, 2);

    EXPECT_EQ(matrix.getRows(), 3);
    EXPECT_EQ(matrix.getCols(), 3);

    for (int i = 0; i < matrix.getRows(); i++) {
        for (int j = 0; j < matrix.getCols(); j++) {
            ImageBlock block = matrix.getBlockAt(i, j);

            EXPECT_EQ(block.Y.rows, 8);
            EXPECT_EQ(block.Y.cols, 8);
            EXPECT_EQ(countNonZero(block.Y - 30), 0);

            EXPECT_EQ(block.Cb.rows, 4);
            EXPECT_EQ(block.Cb.cols, 4);
            EXPECT_EQ(countNonZero(block.Cb - 60), 0);

            EXPECT_EQ(block.Cr.rows, 4);
            EXPECT_EQ(block.Cr.cols, 4);
            EXPECT_EQ(countNonZero(block.Cr - 90), 0);
        }
    }
}

TEST(ImageBlockMatrixTest, SingleBlockImage) {
    Mat image(8, 8, CV_8UC3, Scalar(255, 128, 64));
    ImageBlockMatrix matrix(image, 2);

    EXPECT_EQ(matrix.getRows(), 1);
    EXPECT_EQ(matrix.getCols(), 1);

    ImageBlock block = matrix.getBlockAt(0, 0);
    EXPECT_EQ(block.Y.rows, 8);
    EXPECT_EQ(block.Y.cols, 8);
    EXPECT_EQ(countNonZero(block.Y - 255), 0);

    EXPECT_EQ(block.Cb.rows, 4);
    EXPECT_EQ(block.Cb.cols, 4);
    EXPECT_EQ(countNonZero(block.Cb - 128), 0);

    EXPECT_EQ(block.Cr.rows, 4);
    EXPECT_EQ(block.Cr.cols, 4);
    EXPECT_EQ(countNonZero(block.Cr - 64), 0);
}

TEST(ImageBlockMatrixTest, MinimumSizeImage) {
    Mat image(1, 1, CV_8UC3, Scalar(10, 20, 30));
    ImageBlockMatrix matrix(image, 2);

    EXPECT_EQ(matrix.getRows(), 1);
    EXPECT_EQ(matrix.getCols(), 1);

    ImageBlock block = matrix.getBlockAt(0, 0);
    EXPECT_EQ(block.Y.rows, 8);
    EXPECT_EQ(block.Y.cols, 8);
    EXPECT_EQ(countNonZero(block.Y - 10), 0);

    EXPECT_EQ(block.Cb.rows, 4);
    EXPECT_EQ(block.Cb.cols, 4);
    EXPECT_EQ(countNonZero(block.Cb - 20), 0);

    EXPECT_EQ(block.Cr.rows, 4);
    EXPECT_EQ(block.Cr.cols, 4);
    EXPECT_EQ(countNonZero(block.Cr - 30), 0);
}

TEST(ImageBlockMatrixTest, LargeNonDivisibleImageWithSubsampling) {
    Mat image(35, 37, CV_8UC3, Scalar(75, 125, 175));
    ImageBlockMatrix matrix(image, 2);

    EXPECT_EQ(matrix.getRows(), 5);
    EXPECT_EQ(matrix.getCols(), 5);

    for (int i = 0; i < matrix.getRows(); i++) {
        for (int j = 0; j < matrix.getCols(); j++) {
            ImageBlock block = matrix.getBlockAt(i, j);

            EXPECT_EQ(block.Y.rows, 8);
            EXPECT_EQ(block.Y.cols, 8);
            EXPECT_EQ(countNonZero(block.Y - 75), 0);

            EXPECT_EQ(block.Cb.rows, 4);
            EXPECT_EQ(block.Cb.cols, 4);
            EXPECT_EQ(countNonZero(block.Cb - 125), 0);

            EXPECT_EQ(block.Cr.rows, 4);
            EXPECT_EQ(block.Cr.cols, 4);
            EXPECT_EQ(countNonZero(block.Cr - 175), 0);
        }
    }
}


//TEST(ImageBlockMatrixTest, ReconstructionWithSubsampling) {
//    Mat image(256, 256, CV_8UC3);
//    randu(image, Scalar(0, 0, 0), Scalar(255, 255, 255));
//
//    ImageBlockMatrix matrix(image, 2);
//    Mat reconstructed = matrix.reconstructImage(matrix);
//
//    EXPECT_TRUE(areImagesEqual(image, reconstructed, 5.0));
//}
//
//TEST(ImageBlockMatrixTest, ReconstructionWithPaddingAndSubsampling) {
//    Mat image(19, 21, CV_8UC3);
//    randu(image, Scalar(0, 0, 0), Scalar(255, 255, 255));
//
//    ImageBlockMatrix matrix(image, 2);
//    Mat reconstructed = matrix.reconstructImage(matrix);
//
//    Rect validRegion(0, 0, 21, 19);
//    EXPECT_TRUE(areImagesEqual(image(validRegion), reconstructed(validRegion), 5.0));
//}
