#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "dct.h"
#include "inputs.h"
#include "outputs.h"

#include "testHelpers.cpp"

void validateDCTOutput(const float input[8][8], const float expectedOutput[8][8], const std::string& testName) {
    Mat expectedDCTMat(8, 8, CV_32F, (void*)expectedOutput);

    ImageBlock block;
    block.Y = Mat(8, 8, CV_32F, (void*)input).clone();
    block.Cb = Mat::zeros(4, 4, CV_32F);
    block.Cr = Mat::zeros(4, 4, CV_32F);

    applyDCT(block);

    EXPECT_TRUE(areImagesEqual<float>(block.Y, expectedDCTMat, 1.0))
                        << "DCT output does not match expected values for test: " << testName;
}

void testDCTAndQuantization(const float input[8][8], const float expectedOutput[8][8], const char expectedQuantization[8][8], const std::string& testName) {
    Mat expectedDCTMat(8, 8, CV_32F, (void*)expectedOutput);
    Mat expectedQuantMat(8, 8, CV_8S, (void*)expectedQuantization);
    ImageBlock block;
    block.Y = Mat(8,8,CV_32F,(void*)input).clone();
    block.Cb = Mat::zeros(8, 8, CV_32F);
    block.Cr = Mat::zeros(8, 8, CV_32F);

    applyDCT(block);

    EXPECT_TRUE(areImagesEqual<float>(block.Y, expectedDCTMat, 1.0))
                        << testName << " - DCT output does not match expected values!";

    quantizeBlock(block);

    EXPECT_TRUE(areImagesEqual<char>(block.Y, expectedQuantMat, 1.0))
                        << testName << " - Quantization output does not match expected values!";
}



TEST(DCTTest, ValidateDCTOnAllChannels) {
    Mat inputY  = generateRandomMatrix(8, 8,CV_32F);
    Mat inputCb = generateRandomMatrix(8, 8, CV_32F);
    Mat inputCr = generateRandomMatrix(8, 8, CV_32F);

    ImageBlock block(inputY,inputCb,inputCr);

    Mat expectedY, expectedCb, expectedCr;
    resize(inputCb, inputCb,Size(8,8), 0,0,INTER_LINEAR);
    resize(inputCr, inputCr,Size(8,8), 0,0,INTER_LINEAR);

    inputY -= 128.0f;
    inputCb -= 128.0f;
    inputCr -= 128.0f;

    dct(inputY, expectedY);
    dct(inputCb, expectedCb);
    dct(inputCr, expectedCr);

    applyDCT(block);

    EXPECT_TRUE(areImagesEqual<float>(block.Y, expectedY, 1e-3));
    EXPECT_TRUE(areImagesEqual<float>(block.Cb, expectedCb, 1e-3));
    EXPECT_TRUE(areImagesEqual<float>(block.Cr, expectedCr, 1e-3));
}

TEST(DCTTest, ValidateDCTOutput_1) {
    validateDCTOutput(dctInput_1, dctOutput_1, "Test Case 1");
}

TEST(DCTTest, ValidateDCTOutput_2) {
    validateDCTOutput(dctInput_2, dctOutput_2, "Test Case 2");
}

TEST(DCTTest,ValidateDCT_Output_3) {
    validateDCTOutput(dctInput_3,dctOutput_3,"Test Case 3");
}

TEST(DCTTest,ValidateDCT_Output_4) {
    testDCTAndQuantization(quantizedDctInput_1,dctOutput_4,quantizedDctOutput_1,"Quantization Test Case 1");
}

TEST(DCTTest,ValidateDCT_Output_5) {
    testDCTAndQuantization(quantizedDctInput_2,dctOutput_5,quantizedDctOutput_2,"Quantization Test Case 1");
}
