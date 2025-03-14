#include "ImageBlockMatrix.h"

using namespace cv;

ImageBlockMatrix::ImageBlockMatrix(const int rows, const int cols, int subsampleRatio)
        : m_rows(rows), m_cols(cols), m_blockMatrix(rows * cols, ImageBlock(subsampleRatio)) {}

ImageBlock ImageBlockMatrix::getBlockAt(const int rowIndex, int colIndex) const {
    if (rowIndex < 0 || rowIndex >= m_rows || colIndex < 0 || colIndex >= m_cols) {
        throw std::out_of_range("Block index out of bounds");
    }
    return m_blockMatrix[rowIndex * m_cols + colIndex];
}

void ImageBlockMatrix::setBlockAt(const int rowIndex, int colIndex, const ImageBlock& block) {
    if (rowIndex < 0 || rowIndex >= m_rows || colIndex < 0 || colIndex >= m_cols) {
        throw std::out_of_range("Block index out of bounds");
    }
    m_blockMatrix[rowIndex * m_cols + colIndex] = block;
}

ImageBlockMatrix::ImageBlockMatrix(Mat img, int subsampleRatio) {
    std::vector<Mat> channels;
    split(img, channels);
    Mat Y = channels[0], Cb = channels[1], Cr = channels[2];

    // Compute number of blocks
    m_rows = Y.rows / 8;
    m_cols = Y.cols / 8;
    m_blockMatrix.resize(m_rows * m_cols, ImageBlock(subsampleRatio));

    int chromaSize = 8 / subsampleRatio;

    // Resize (subsample) chroma channels before splitting into blocks
    Mat CbSubsampled, CrSubsampled;
    resize(Cb, CbSubsampled, Size(Cb.cols / subsampleRatio, Cb.rows / subsampleRatio), 0, 0, INTER_LINEAR);
    resize(Cr, CrSubsampled, Size(Cr.cols / subsampleRatio, Cr.rows / subsampleRatio), 0, 0, INTER_LINEAR);

    // Now split into blocks
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            ImageBlock& block = m_blockMatrix[i * m_cols + j];  // FIXED INDEX

            // Extract 8x8 Y block
            block.Y = Y(Rect(j * 8, i * 8, 8, 8)).clone();

            // Extract 4x4 (for subsampleRatio=2) chroma blocks
            block.Cb = CbSubsampled(Rect(j * chromaSize, i * chromaSize, chromaSize, chromaSize)).clone();
            block.Cr = CrSubsampled(Rect(j * chromaSize, i * chromaSize, chromaSize, chromaSize)).clone();
        }
    }
}


int ImageBlockMatrix::getCols() const {
    return m_cols;
}

int ImageBlockMatrix::getRows() const {
    return m_rows;
}

cv::Mat ImageBlockMatrix::reconstructImage(ImageBlockMatrix blockMatrix) {
    return cv::Mat();
}
