#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "myfunction.hpp"



bool make_gray = false;
bool make_color = true;

bool make_leha = true;
bool make_cat = true;
bool make_cube = true;


int main(int argc, char** argv) {
    
    cv::Mat cube = cv::imread("src/cube.png");
    cv::Mat cat = cv::imread("src/cat.png");
    cv::Mat leha = cv::imread("src/leha.jpg");
 
    cv::Mat catGray;
    cv::Mat lehaGray;
    cv::Mat cubeGray;

    myCvtColorToGray(cat, catGray);
    myCvtColorToGray(leha, lehaGray);
    myCvtColorToGray(cube, cubeGray);

    cv::Mat result;
 
    int n_bit;
    std::string filename_1;
   

    if (make_gray) {
        if (make_cube) {
            filename_1 = "src/result/gray/cube/stuki_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myGrayStuckiDithering(cubeGray, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }

            filename_1 = "src/result/gray/cube/floyd_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myGrayFloydSteinbergDithering(cubeGray, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }
        }

        if (make_cat) {
            filename_1 = "src/result/gray/cat/stuki_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myGrayStuckiDithering(catGray, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }

            filename_1 = "src/result/gray/cat/floyd_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myGrayFloydSteinbergDithering(catGray, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }
        }

        if (make_leha) {
            filename_1 = "src/result/gray/leha/stuki_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myGrayStuckiDithering(lehaGray, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }

            filename_1 = "src/result/gray/leha/floyd_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myGrayFloydSteinbergDithering(lehaGray, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }
        }
    }

    if (make_color) {
        
        if (make_cat) {
            filename_1 = "src/result/color/cat/stuki_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myColorStuckiDithering(cat, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }

            filename_1 = "src/result/color/cat/floyd_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myColorFloydSteinbergDithering(cat, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }
        }

        if (make_leha) {
            filename_1 = "src/result/color/leha/stuki_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myColorStuckiDithering(leha, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }

            filename_1 = "src/result/color/leha/floyd_";
            for (n_bit = 1; n_bit < 5; ++n_bit) {
                result = myColorFloydSteinbergDithering(leha, n_bit);
                cv::imwrite(filename_1 + std::to_string(n_bit) + "bit.png", result);
            }
        }
    }

  

    cv::waitKey(0);
    return 0;
}
