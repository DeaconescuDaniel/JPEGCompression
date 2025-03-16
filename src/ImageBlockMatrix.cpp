#include "ImageBlockMatrix.h"

using namespace cv;

ImageBlockMatrix::ImageBlockMatrix(const int rows, const int cols, int subsampleRatio)
        : m_rows(rows), m_cols(cols), m_subsampleRatio(subsampleRatio), m_blockMatrix(rows * cols, ImageBlock(subsampleRatio)) {}

ImageBlock ImageBlockMatrix::getBlockAt(const int rowIndex, const int colIndex) const {
    if (rowIndex < 0 || rowIndex >= m_rows || colIndex < 0 || colIndex >= m_cols) {
        throw std::out_of_range("Block index out of bounds");
    }
    return m_blockMatrix[rowIndex * m_rows + colIndex];
}

void ImageBlockMatrix::setBlockAt(const int rowIndex, const int colIndex, const ImageBlock& block) {
    if (rowIndex < 0 || rowIndex >= m_rows || colIndex < 0 || colIndex >= m_cols) {
        throw std::out_of_range("Block index out of bounds");
    }
    m_blockMatrix[rowIndex * m_rows + colIndex] = block;
}

ImageBlockMatrix::ImageBlockMatrix(const Mat& img, int subsampleRatio) : m_subsampleRatio(subsampleRatio) {
    assert(img.channels() == 3);

    int paddedWidth = (img.cols % 8 == 0) ? img.cols : img.cols + (8 - img.cols % 8);
    int paddedHeight = (img.rows % 8 == 0) ? img.rows : img.rows + (8 - img.rows % 8);

    Mat paddedImg;
    copyMakeBorder(img, paddedImg, 0, paddedHeight - img.rows, 0, paddedWidth - img.cols, BORDER_REPLICATE);

    std::vector<Mat> channels;
    split(paddedImg, channels);
    Mat Y = channels[0], Cb = channels[1], Cr = channels[2];

    m_rows = paddedHeight / 8;
    m_cols = paddedWidth / 8;
    m_blockMatrix.resize(m_rows * m_cols, ImageBlock(subsampleRatio));

    int chromaSize = 8 / subsampleRatio;

    Mat CbSubsampled, CrSubsampled;
    resize(Cb, CbSubsampled, Size(paddedWidth / subsampleRatio, paddedHeight / subsampleRatio), 0, 0, INTER_LINEAR);
    resize(Cr, CrSubsampled, Size(paddedWidth / subsampleRatio, paddedHeight / subsampleRatio), 0, 0, INTER_LINEAR);

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            ImageBlock& block = m_blockMatrix[i * m_rows + j];

            block.Y = Y(Rect(i * 8, j * 8, 8, 8)).clone();

            block.Cb = CbSubsampled(Rect(i * chromaSize, j * chromaSize, chromaSize, chromaSize)).clone();
            block.Cr = CrSubsampled(Rect(i * chromaSize, j * chromaSize, chromaSize, chromaSize)).clone();
        }
    }
}

Mat ImageBlockMatrix::reconstructImage(ImageBlockMatrix blockMatrix) const {
    int height = blockMatrix.getRows() * 8;
    int width = blockMatrix.getCols() * 8;
    int chromaSize = blockMatrix.m_blockMatrix[0].Cb.rows;

    Mat Y(height, width, CV_8UC1);
    Mat Cb(height / m_subsampleRatio, width / m_subsampleRatio, CV_8UC1);
    Mat Cr(height / m_subsampleRatio, width / m_subsampleRatio, CV_8UC1);


    for (int i = 0; i < blockMatrix.getRows(); i++) {
        for (int j = 0; j < blockMatrix.getCols(); j++) {
            const ImageBlock& block = blockMatrix.getBlockAt(i,j);

            block.Y.copyTo(Y(Rect(i * 8, j * 8, 8, 8)));
            block.Cb.copyTo(Cb(Rect(j * chromaSize, i * chromaSize, chromaSize, chromaSize)));
            block.Cr.copyTo(Cr(Rect(j * chromaSize, i * chromaSize, chromaSize, chromaSize)));
        }
    }

    Mat CbUpsampled, CrUpsampled;
    resize(Cb, CbUpsampled, Size(width, height), 0, 0, INTER_LINEAR);
    resize(Cr, CrUpsampled, Size(width, height), 0, 0, INTER_LINEAR);

    Mat reconstructedImage;
    std::vector<Mat> channels = {Y, CbUpsampled, CrUpsampled};
    merge(channels, reconstructedImage);

    return reconstructedImage;
}

int ImageBlockMatrix::getCols() const {
    return m_cols;
}

int ImageBlockMatrix::getRows() const {
    return m_rows;
}

