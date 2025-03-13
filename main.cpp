#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;

Mat YCCtoRGB(Mat imgYCC) {
    const int rows = imgYCC.rows, cols = imgYCC.cols;
    Mat imgRGB(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b YCC = imgYCC.at<Vec3b>(i, j);
            uchar Y = YCC[0], Cb = YCC[1], Cr = YCC[2];

            int R = Y + 1.402 * (Cr - 128);
            int G = Y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128);
            int B = Y + 1.772 * (Cb - 128);

            imgRGB.at<Vec3b>(i, j) = Vec3b(
                    std::clamp(B, 0, 255),
                    std::clamp(G, 0, 255),
                    std::clamp(R, 0, 255)
            );
        }
    }
    return imgRGB;
}


Mat RGBtoYCC(Mat imgRGB) {
    const int rows = imgRGB.rows, cols = imgRGB.cols;
    Mat imgYCC(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b BGR = imgRGB.at<Vec3b>(i, j);
            uchar B = BGR[0], G = BGR[1], R = BGR[2];

            auto Y  = static_cast<uchar>(0.299 * R + 0.587 * G + 0.114 * B);
            auto Cb = static_cast<uchar>(128 + (-0.168736 * R - 0.331264 * G + 0.5 * B));
            auto Cr = static_cast<uchar>(128 + (0.5 * R - 0.418688 * G - 0.081312 * B));

            imgYCC.at<Vec3b>(i, j) = Vec3b(Y, Cb, Cr);
        }
    }
    return imgYCC;
}



int main() {
    Mat img = imread("..\\Test\\Lena_24bits.bmp",IMREAD_COLOR);
    imshow("RGB",img);
    imshow("YCC->RGB", YCCtoRGB(RGBtoYCC(img)));
    waitKey(0);
    return 0;
}
