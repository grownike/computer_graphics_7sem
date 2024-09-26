#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>


float D(float x) {
    if (x <= 0.25) {
        return x * (4 + x * (16 * x - 12));
    }
    else {
        return sqrt(x);
    }
}

unsigned char B(unsigned char p1, unsigned char p2, int MODE) {
    float p1_norm = p1 / 255.0f;
    float p2_norm = p2 / 255.0f;
    float result = 0.0f;

    if (MODE == 1) {
        result = p2_norm;
    }
    if (MODE == 2) {
        result = p1_norm * p2_norm;
    }
    if (MODE == 3) {
        result = 1 - (1 - p1_norm) * (1 - p2_norm);
    }
    if (MODE == 4) {
        result = std::min(p1_norm, p2_norm);
    }
    if (MODE == 5) {
        result = std::max(p1_norm, p2_norm);
    }
    if (MODE == 6) {
        result = std::abs(p1_norm - p2_norm);
    }
    if (MODE == 7) {
        if (p2_norm == 1) {
            result = 1;
        }
        else {
            result = std::min(1.0f, p1_norm / (1 - p2_norm));
        }
    }
    if (MODE == 8) {
        if (p2_norm == 0) {
            result = 0;
        }
        else {
            result = 1.0f - std::min(1.0f, (1 - p1_norm) / p2_norm);
        }
    }
    if (MODE == 9) {
        if (p2_norm <= 0.5) {
            result = p1_norm - (1 - 2 * p2_norm) * p1_norm * (1 - p1_norm);
        }
        else {
            result = p1_norm + (2 * p2_norm - 1) * (D(p1_norm) - p1_norm);
        }
    }
    return static_cast<unsigned char>(result * 255);
}


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

void myApplyMask(const cv::Mat& src, const cv::Mat& mask, cv::Mat& dst) {
    
    if (src.size() != mask.size() || mask.channels() != 1) {
        std::cerr << "error wrong size or mask" << std::endl;
        return;
    }
    
    
    dst = cv::Mat(src.size(), src.type(), cv::Scalar(255, 255, 255));

    
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            unsigned char maskPixel = mask.at<unsigned char>(i, j);
            if (maskPixel == 255) {
                if (src.channels() == 1) dst.at<unsigned char>(i, j) = src.at<unsigned char>(i, j);
                if (src.channels() == 2) dst.at<cv::Vec2b>(i, j) = src.at<cv::Vec2b>(i, j);
                if (src.channels() == 3) dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, j);
                if (src.channels() == 4) dst.at<cv::Vec4b>(i, j) = src.at<cv::Vec4b>(i, j);
            }
        }
    }
}

void myBlendImages(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& result, int MODE) {
    if (MODE <= 0 or MODE >= 10) {
        std::cerr << "error: unknow mode" << std::endl;
        return;
    }

    if (img1.size() != img2.size()) {
        std::cerr << "error: images must have same size" << std::endl;
        return;
    }
    if (img1.type() != CV_8UC4 && img2.type() != CV_8UC4) {
        std::cerr << "error: images must have 8 bit and 4 chanels" << std::endl;
        return;
    }

    result = cv::Mat(img1.size(), img1.type());

    
    for (int i = 0; i < img1.rows; ++i) {
        for (int j = 0; j < img1.cols; ++j) {
            
            cv::Vec4b pixel1 = img1.at<cv::Vec4b>(i, j);
            cv::Vec4b pixel2 = img2.at<cv::Vec4b>(i, j);


            float alpha1 = pixel1[3] / 255.0f;
            float alpha2 = pixel2[3] / 255.0f;

            
            unsigned char pixel1R = static_cast<unsigned char>(pixel1[0]);
            unsigned char pixel1G = static_cast<unsigned char>(pixel1[1]);
            unsigned char pixel1B = static_cast<unsigned char>(pixel1[2]);

            unsigned char pixel2R = static_cast<unsigned char>(pixel2[0]);
            unsigned char pixel2G = static_cast<unsigned char>(pixel2[1]);
            unsigned char pixel2B = static_cast<unsigned char>(pixel2[2]);

            unsigned char newR = (1 - alpha1) * alpha2 * pixel2R + (1 - alpha2) * alpha1 * pixel1R + alpha1 * alpha2 * B(pixel1R, pixel2R, MODE);
            unsigned char newG = (1 - alpha1) * alpha2 * pixel2G + (1 - alpha2) * alpha1 * pixel1G + alpha1 * alpha2 * B(pixel1G, pixel2G, MODE);
            unsigned char newB = (1 - alpha1) * alpha2 * pixel2B + (1 - alpha2) * alpha1 * pixel1B + alpha1 * alpha2 * B(pixel1B, pixel2B, MODE);
           
            result.at<cv::Vec4b>(i, j) = cv::Vec4b(newR, newG, newB, 255);
        }
    }
}

void myBlendImagesWitmMask(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& alpha, cv::Mat& result) {
    
    if (img1.size() != img2.size() || img1.size() != alpha.size()) {
        std::cerr << "error wrong size" << std::endl;
        return;
    }
    
    result = cv::Mat(img1.size(), img1.type());

    
    for (int i = 0; i < img1.rows; ++i) {
        for (int j = 0; j < img1.cols; ++j) {
            
            float alphaValue = alpha.at<unsigned char>(i, j) / 255.0f;

            if (img1.channels() == 1) { 
                unsigned char pixel1 = img1.at<unsigned char>(i, j);
                unsigned char pixel2 = img2.at<unsigned char>(i, j);
                unsigned char newPixel = static_cast<unsigned char>(alphaValue * pixel1 + (1 - alphaValue) * pixel2);
                result.at<unsigned char>(i, j) = newPixel;
            }
            if (img1.channels() == 3) { 
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(i, j);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(i, j);
                unsigned char newR = static_cast<unsigned char>(alphaValue * pixel1[0] + (1 - alphaValue) * pixel2[0]);
                unsigned char newG = static_cast<unsigned char>(alphaValue * pixel1[1] + (1 - alphaValue) * pixel2[1]);
                unsigned char newB = static_cast<unsigned char>(alphaValue * pixel1[2] + (1 - alphaValue) * pixel2[2]);
                result.at<cv::Vec3b>(i, j) = cv::Vec3b(newR, newG, newB);
            }
            if (img1.channels() == 4) {
                cv::Vec4b pixel1 = img1.at<cv::Vec4b>(i, j);
                cv::Vec4b pixel2 = img2.at<cv::Vec4b>(i, j);
                unsigned char newR = static_cast<unsigned char>(alphaValue * pixel1[0] + (1 - alphaValue) * pixel2[0]);
                unsigned char newG = static_cast<unsigned char>(alphaValue * pixel1[1] + (1 - alphaValue) * pixel2[1]);
                unsigned char newB = static_cast<unsigned char>(alphaValue * pixel1[2] + (1 - alphaValue) * pixel2[2]);
                result.at<cv::Vec4b>(i, j) = cv::Vec4b(newR, newG, newB, 255);
            }
        }
    }
}

void myCreateAlpha(const cv::Mat& img1, const cv::Mat& mask, cv::Mat& result) {
    
    if (img1.size() != mask.size()) {
        std::cerr << "error: images must have same size" << std::endl;
        return;
    }
   
    result = cv::Mat(img1.size(), img1.type());


    for (int i = 0; i < img1.rows; ++i) {
        for (int j = 0; j < img1.cols; ++j) {

            
            cv::Vec4b pixel1 = img1.at<cv::Vec4b>(i, j);
            result.at<cv::Vec4b>(i, j) = cv::Vec4b(pixel1[0], pixel1[1], pixel1[2], mask.at<unsigned char>(i, j));
        }
    }
}

void myTransposeImage(const cv::Mat& src, cv::Mat& dst) {
    dst = cv::Mat(src.cols, src.rows, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            if (src.channels() == 1) dst.at<unsigned char>(j, i) = src.at<unsigned char>(i, j);
            if (src.channels() == 3) dst.at<cv::Vec3b>(j, i) = src.at<cv::Vec3b>(i, j);
            if (src.channels() == 4) dst.at<cv::Vec4b>(j, i) = src.at<cv::Vec4b>(i, j);
        }
    }
}

void myMirrorImageHorizontally(const cv::Mat& src, cv::Mat& dst) {
 
    dst = cv::Mat(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            if (src.channels() == 1) dst.at<unsigned char>(i, j) = src.at<unsigned char>(i, src.cols - 1 - j);
            if (src.channels() == 3) dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(i, src.cols - 1 - j);
            if (src.channels() == 4) dst.at<cv::Vec4b>(i, j) = src.at<cv::Vec4b>(i, src.cols - 1 - j);
        }
    }
}

void muMirrorImageVertically(const cv::Mat& src, cv::Mat& dst) {
    dst = cv::Mat(src.rows, src.cols, src.type());
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            if (src.channels() == 1) dst.at<unsigned char>(i, j) = src.at<unsigned char>(src.rows - 1 - i, j);
            if (src.channels() == 3) dst.at<cv::Vec3b>(i, j) = src.at<cv::Vec3b>(src.rows - 1 - i, j);
            if (src.channels() == 4) dst.at<cv::Vec4b>(i, j) = src.at<cv::Vec4b>(src.rows - 1 - i, j);
        }
    }
}
