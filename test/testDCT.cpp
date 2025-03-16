#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "dct.h"
#include "inputs.h"
#include "outputs.h"

#include "testHelpers.cpp"

TEST(DCTTest, UniformBlockTest) {
    ImageBlock block(2, CV_32F);
    block.Y = Mat::ones(8, 8, CV_32F) * 128;
    block.Cb = Mat::ones(4, 4, CV_32F) * 100;
    block.Cr = Mat::ones(4, 4, CV_32F) * 150;

    ImageBlock transformedBlock = applyDCT(block);

    // Check that only the top-left (DC) coefficient is nonzero
    EXPECT_NEAR(transformedBlock.Y.at<float>(0, 0), 1024, 1e-5);
    EXPECT_NEAR(transformedBlock.Cb.at<float>(0, 0), 400, 1e-5);
    EXPECT_NEAR(transformedBlock.Cr.at<float>(0, 0), 600, 1e-5);

    // Ensure all other values are close to zero
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 0 && j == 0) continue;
            EXPECT_NEAR(transformedBlock.Y.at<float>(i, j), 0, 1e-5);
            }
    }
}

TEST(DCTTest, ValidateDCTOutput) {
    // Convert input and expected matrices to OpenCV Mat format
    Mat expectedDCTMat(8, 8, CV_32F, dctOutput_1);

    ImageBlock block;
    block.Y = Mat(8, 8, CV_32F, dctInput_1);
    block.Cb = Mat::zeros(4,4,CV_32F);
    block.Cr = Mat::zeros(4,4,CV_32F);

    applyDCT(block);

    // Compare results
    EXPECT_TRUE(areImagesEqual(block.Y, expectedDCTMat,1.0)) << "DCT output does not match expected values!";
}

TEST(DCTTest, ValidateDCTOnAllChannels) {
    // Generate random 8×8 luma (Y) and 4×4 chroma (Cb, Cr) matrices
    Mat inputY  = generateRandomMatrix(8, 8,CV_32F);
    Mat inputCb = generateRandomMatrix(4, 4, CV_32F);
    Mat inputCr = generateRandomMatrix(4, 4, CV_32F);

    ImageBlock block(inputY,inputCb,inputCr);

    // Compute expected DCT results using OpenCV
    Mat expectedY  = inputY.clone() -128;
    Mat expectedCb = inputCb.clone() -128;
    Mat expectedCr = inputCr.clone() -128;

    dct(expectedY, expectedY);
    dct(expectedY, expectedCb);
    dct(expectedY, expectedCr);

    applyDCT(block);

    // Validate Y channel
    EXPECT_TRUE(areImagesEqual(block.Y, expectedY, 1e-3))
                        << "DCT output for Y channel does not match expected values!";

    // Validate Cb channel
    EXPECT_TRUE(areImagesEqual(block.Cb, expectedCb, 1e-3))
                        << "DCT output for Cb channel does not match expected values!";

    printImage(block.Cb);
    std::cout << "-----------"<<std::endl;
    printImage(expectedCb);

    // Validate Cr channel
    EXPECT_TRUE(areImagesEqual(block.Cr, expectedCr, 1e-3))
                        << "DCT output for Cr channel does not match expected values!";
}