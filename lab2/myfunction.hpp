#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

void myCvtColorToGray(const cv::Mat& src, cv::Mat& dst) {
    dst = cv::Mat(src.rows, src.cols, CV_8UC1);
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            if (src.channels() == 3) {
                cv::Vec3b bgrPixel = src.at<cv::Vec3b>(i, j);
                unsigned char blue = bgrPixel[0];
                unsigned char green = bgrPixel[1];
                unsigned char red = bgrPixel[2];
                unsigned char grayValue = static_cast<unsigned char>(0.114 * blue + 0.587 * green + 0.299 * red);
                dst.at<unsigned char>(i, j) = grayValue;
            }
            if (src.channels() == 4) {
                cv::Vec4b bgrPixel = src.at<cv::Vec4b>(i, j);
                unsigned char blue = bgrPixel[0];
                unsigned char green = bgrPixel[1];
                unsigned char red = bgrPixel[2];

                unsigned char grayValue = static_cast<unsigned char>(0.114 * blue + 0.587 * green + 0.299 * red);
                dst.at<unsigned char>(i, j) = grayValue;
            }
        }
    }
}

uchar findClosestPaletteColor(uchar value, const std::vector<uchar>& palette) {
    uchar closestColor = palette[0];
    int minDifference = std::abs(value - palette[0]);
    for (const uchar& color : palette) {
        int difference = std::abs(value - color);
        if (difference < minDifference) {
            minDifference = difference;
            closestColor = color;
        }
    }
    return closestColor;
}



cv::Mat myGrayFloydSteinbergDithering(cv::Mat& src, int n) {
    cv::Mat image = src.clone();

    int count_color = (int)pow(2, n);
    int step = 255 / (count_color-1);

    std::vector<uchar> palette;
    for (int i = 0; i < count_color; ++i) {
        palette.push_back(i * step);
    }


    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            uchar oldPixel = image.at<uchar>(y, x);
            uchar newPixel = findClosestPaletteColor(oldPixel, palette);
            image.at<uchar>(y, x) = newPixel;
            int error = oldPixel - newPixel;

            float distribution[2][3] = {
                {0, 0, 7},
                {3, 5, 1},
            };
            int divisor = 16;


            for (int j = 0; j < 2; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int newY = y + j;
                    int newX = x + i;
                    if (newY >= 0 && newY < image.rows && newX >= 0 && newX < image.cols) {
                        image.at<uchar>(newY, newX) = cv::saturate_cast<uchar>(
                            image.at<uchar>(newY, newX) + error * distribution[j][i + 1] / divisor
                        );
                    }
                }
            }
        }
    }

    return image;
}

cv::Mat myColorFloydSteinbergDithering(cv::Mat& src, int n) {
    cv::Mat image = src.clone();

    int count_color = (int)pow(2, n);
    int step = 255 / (count_color - 1);

    std::vector<uchar> palette;
    for (int i = 0; i < count_color; ++i) {
        palette.push_back(i * step);
    }

    
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            
            cv::Vec3b oldPixel = image.at<cv::Vec3b>(y, x);

            cv::Vec3b newPixel;
            int errorR, errorG, errorB;

            newPixel[0] = findClosestPaletteColor(oldPixel[0], palette);
            errorR = oldPixel[0] - newPixel[0];

            
            newPixel[1] = findClosestPaletteColor(oldPixel[1], palette);
            errorG = oldPixel[1] - newPixel[1];

            
            newPixel[2] = findClosestPaletteColor(oldPixel[2], palette);
            errorB = oldPixel[2] - newPixel[2];
            
            image.at<cv::Vec3b>(y, x) = newPixel;

            float distribution[2][3] = {
                {0, 0, 7},
                {3, 5, 1},
            };
            int divisor = 16;

            for (int j = 0; j < 2; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int newY = y + j;
                    int newX = x + i;
                    if (newY >= 0 && newY < image.rows && newX >= 0 && newX < image.cols) {
                        image.at<cv::Vec3b>(newY, newX)[0] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(newY, newX)[0] + errorR * distribution[j][i + 1] / divisor);
                        image.at<cv::Vec3b>(newY, newX)[1] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(newY, newX)[1] + errorG * distribution[j][i + 1] / divisor);
                        image.at<cv::Vec3b>(newY, newX)[2] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(newY, newX)[2] + errorB * distribution[j][i + 1] / divisor);
                    }
                }
            }
        }
    }

    return image;
}


cv::Mat myColorStuckiDithering(cv::Mat& src, int n) {
    cv::Mat image = src.clone();

    int count_color = (int)pow(2, n);
    int step = 255 / (count_color - 1);

    std::vector<uchar> palette;
    for (int i = 0; i < count_color; ++i) {
        palette.push_back(i * step);
    }

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec3b oldPixel = image.at<cv::Vec3b>(y, x);

            cv::Vec3b newPixel;
            int errorR, errorG, errorB;

            newPixel[0] = findClosestPaletteColor(oldPixel[0], palette);
            errorR = oldPixel[0] - newPixel[0];

            newPixel[1] = findClosestPaletteColor(oldPixel[1], palette);
            errorG = oldPixel[1] - newPixel[1];

            newPixel[2] = findClosestPaletteColor(oldPixel[2], palette);
            errorB = oldPixel[2] - newPixel[2];

            
            image.at<cv::Vec3b>(y, x) = newPixel;

            
            float distribution[3][5] = {
                {0, 0, 0, 8, 4},
                {2, 4, 8, 4, 2},
                {1, 2, 4, 2, 1}
            };
            int divisor = 42;

            
            for (int j = 0; j < 3; ++j) {
                for (int i = -2; i <= 2; ++i) {
                    int newY = y + j;
                    int newX = x + i;
                    if (newY >= 0 && newY < image.rows && newX >= 0 && newX < image.cols) {
                        image.at<cv::Vec3b>(newY, newX)[0] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(newY, newX)[0] + errorR * distribution[j][i + 2] / divisor);
                        image.at<cv::Vec3b>(newY, newX)[1] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(newY, newX)[1] + errorG * distribution[j][i + 2] / divisor);
                        image.at<cv::Vec3b>(newY, newX)[2] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(newY, newX)[2] + errorB * distribution[j][i + 2] / divisor);
                    }
                }
            }
        }
    }

    return image;
}

cv::Mat myGrayStuckiDithering(cv::Mat& src, int n) {
    cv::Mat image = src.clone();

    int count_color = (int)pow(2, n);
    int step = 255 / (count_color - 1);

    std::vector<uchar> palette;
    for (int i = 0; i < count_color; ++i) {
        palette.push_back(i * step);
    }


    
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            
            uchar oldPixel = image.at<uchar>(y, x);

            
            uchar newPixel = findClosestPaletteColor(oldPixel, palette);
            int error = oldPixel - newPixel;

            
            image.at<uchar>(y, x) = newPixel;

            float distribution[3][5] = {
                {0, 0, 0, 8, 4},
                {2, 4, 8, 4, 2},
                {1, 2, 4, 2, 1}
            };
            int divisor = 42;

            
            for (int j = 0; j < 3; ++j) {
                for (int i = -2; i <= 2; ++i) {
                    int newY = y + j;
                    int newX = x + i;
                    if (newY >= 0 && newY < image.rows && newX >= 0 && newX < image.cols) {
                        image.at<uchar>(newY, newX) = cv::saturate_cast<uchar>(image.at<uchar>(newY, newX) + error * distribution[j][i + 2] / divisor);
                    }
                }
            }
        }
    }

    return image;
}