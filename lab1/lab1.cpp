#include <opencv2/opencv.hpp>
#include <iostream>
#include "myfunction.hpp"




int main() {

    std::string modes[9] = {
    "normal",       // 1
    "multiply",     // 2
    "screen",       // 3
    "darken",       // 4
    "lighten",      // 5
    "difference",   // 6
    "colorDodge",   // 7
    "colorBurn",    // 8
    "softLight"     // 9
    };


   
    cv::Mat cat = cv::imread("src/cat.png", cv::IMREAD_UNCHANGED);
    cv::Mat flag_rf = cv::imread("src/flag rf with alpha.png", cv::IMREAD_UNCHANGED);
    cv::Mat kurgan = cv::imread("src/kurgan.png", cv::IMREAD_UNCHANGED);
    cv::Mat rainbow = cv::imread("src/rainbow with alpha.png", cv::IMREAD_UNCHANGED);


    cv::Mat circle_mask = cv::imread("src/circle_mask.png", cv::IMREAD_GRAYSCALE);
    cv::Mat grad1 = cv::imread("src/gradient 1.png", cv::IMREAD_GRAYSCALE);
    cv::Mat grad2 = cv::imread("src/gradient 2.png", cv::IMREAD_GRAYSCALE);
   


    std::string filename11 = "src/results/kurgan/";
    std::string filename12 = "_city+rainbow.png";
    cv::Mat results_1[9];
    for (int i = 0; i < 9; i++) {
        myBlendImages(kurgan, rainbow, results_1[i], i + 1);
        std::string filename = filename11 + modes[i] + filename12;
        cv::imwrite(filename, results_1[i]);
    }
    


    std::string filename21 = "src/results/cats/";
    std::string filename22 = "_cat+flagRF.png";
    cv::Mat results_2[9];
    for (int i = 0; i < 9; i++) {
        myBlendImages(cat, flag_rf, results_2[i], i + 1);
        std::string filename = filename21 + modes[i] + filename22;
        cv::imwrite(filename, results_2[i]);
    }


    cv::Mat cat_vert;
    cv::Mat cat_hor;
    cv::Mat cat_trans;
    cv::Mat cat_gray;
    cv::Mat cat_gray_circle_mask;
    cv::Mat cat_circle_mask;



    myCvtColorToGray(cat, cat_gray);
    myApplyMask(cat_gray, circle_mask, cat_gray_circle_mask);
    myApplyMask(cat, circle_mask, cat_circle_mask);



    cv::imwrite("src/results/cats/cat gray.png", cat_gray);
    cv::imwrite("src/results/cats/cat gray with circl mask.png", cat_gray_circle_mask);
    cv::imwrite("src/results/cats/cat with circl mask.png", cat_circle_mask);


    myTransposeImage(cat, cat_trans);
    myMirrorImageHorizontally(cat, cat_hor);
    muMirrorImageVertically(cat, cat_vert);


    cv::imwrite("src/results/cats/catVert.png", cat_vert);
    cv::imwrite("src/results/cats/catHor.png", cat_hor);
    cv::imwrite("src/results/cats/catTrans.png", cat_trans);

    cv::Mat cat_and_city;

    myBlendImagesWitmMask(cat, kurgan, grad1, cat_and_city);
    cv::imwrite("src/results/cats/cat with city.png", cat_and_city);

    cv::waitKey(0);
    return 0;
}