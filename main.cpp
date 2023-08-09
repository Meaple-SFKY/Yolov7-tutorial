//
//  main.cpp
//  PostProcess
//
//  Created by SFKY on 2023/8/9.
//

#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

void viewArr(float *arr, int len);
int min(int num, int data);
float *getAlpha(float sinAlpha, float cosAlpha);
Point getPoint(float xyxy[4], float alpha[2]);

int main(int argc, char** argv)
{
    Mat src = imread("/Users/sfky/Projects/PostProcess/PostProcess/img/img.jpg");

    Rect rect(171, 262, 484, 485);

    // cv::rectangle(src, rect, Scalar(255, 255, 255), 2, LINE_AA, 0);
    cv::rectangle(src, Point(171, 262), Point(484, 485), Scalar(255, 255, 255), 1, LINE_AA, 0);

    float pred[9] = {171, 262, 484, 485, 0.588, 0.6392, -0.754, 0.97682, 0};
    int xyxy[4] = {0};
    float ratio = 0;
    float alpha[2] = {0};
    float targetPoint[2] = {0};
    float conf = 0;
    float tanAlpha = 0;
    float xRatio = 0;
    int cls = 0;
    int width = 0;
    int height = 0;
    int rWidth = 0;
    bool direction = true;

    float *angleList = new float(3);

    for (int i = 0; i < 4; i++) {
        xyxy[i] = int(pred[i]);
    }
    
    ratio = pred[4];
    for (int i = 0; i < 2; i++) {
        alpha[i] = pred[5 + i];
    }
    conf = pred[7];
    cls = int(pred[8]);
    
    width = xyxy[2] - xyxy[0];
    height = xyxy[3] - xyxy[1];

    rWidth = int(width * ratio);

    angleList = getAlpha(alpha[0], alpha[1]);
    
    if (*(angleList + 2) == 0) {
        tanAlpha = 640;
    }
    else {
        tanAlpha = *(angleList + 1) / *(angleList + 2);
    }
    
    cout << tanAlpha << endl;
    
    for (int i = 0; i < 3; i++) {
        cout << *(angleList + i) << endl;
    }
    
    direction = (*angleList > 0);
    
    if (ratio == 1) {
        if (direction) {
            targetPoint[0] = xyxy[0];
            xRatio = xyxy[0];
        }
        else {
            targetPoint[0] = xyxy[2];
            xRatio = xyxy[2];
        }
        targetPoint[1] = xyxy[3];
    }
    else if (ratio == 0) {
//        if (*(angleList + 2) > 0) {
//            targetPoint[0] = xyxy[2];
//            xRatio = xyxy[0];
//            targetPoint[1] = xyxy[3] - min(abs(width * tanAlpha), height);
//        }
//        else if (*(angleList + 2) < 0) {
//            targetPoint[0] = xyxy[0];
//            xRatio = xyxy[2];
//            targetPoint[1] = xyxy[3] - min(abs(width * tanAlpha), height);
//        }
//        else {
//            // exception
//            targetPoint[0] = xyxy[0];
//            xRatio = xyxy[0];
//            targetPoint[1] = xyxy[3];
//        }
    }
    else if (ratio > 0 && ratio < 1) {
//        if (*(angleList + 2) > 0) {
//            targetPoint[0] = xyxy[2];
//            xRatio = xyxy[0] + rWidth;
//            targetPoint[1] = xyxy[3] - min(abs((width - rWidth) * tanAlpha), height);
//        }
//        else if (*(angleList + 2) < 0) {
//            targetPoint[0] = xyxy[0];
//            xRatio = xyxy[2] - rWidth;
//            targetPoint[1] = xyxy[3] - min(abs(rWidth * tanAlpha), height);
//        }
//        else {
//            targetPoint[0] = xyxy[0];
//            xRatio = xyxy[0];
//            targetPoint[1] = xyxy[3];
//        }
    }
    
    cout << targetPoint[0] << ", " << targetPoint[1] << ", " << xRatio << ", " << rWidth << ", " << width << endl;

    cv::imwrite("/Users/sfky/Projects/PostProcess/PostProcess/out_img/test_new.jpg", src);

    return 0;
}

void viewArr(float *arr, int len)
{
    for (int i = 0; i < len; i++) {
        cout << *(arr + i) << ' ';
    }
    cout << endl;
}

int min(int num, int data)
{
    return (num < data) ? num : data;
}

/*
 * index 0: alpha
 * index 1: sin_alpha
 * index 2: cos_alpha
 */
float *getAlpha(float sinAlpha, float cosAlpha)
{
    float r = 0;
    float alphaSin = 0;
    float alphaCos = 0;
    float alpha = 0;
    float *alphaList = new float(3);
    float ratioPi = 57.2957795131;   // equal to 180 / PI, for eliminating division.
    r = 1 / sqrt(pow(sinAlpha, 2) + pow(cosAlpha, 2));

    alphaSin = sinAlpha * r;
    alphaCos = cosAlpha * r;

    if (alphaSin != 0 && alphaCos != 0) {
        alpha = asin(alphaSin) * ratioPi;
        if (alphaCos < 0) {
            alpha = 180 - alpha;
        }
    }
    else if (alphaSin == 0) {
        if (alphaCos == 1) {
            alpha = 0;
        }
        else {
            alpha = 180;
        }
    }
    else {
        if (alphaSin == 1) {
            alpha = 90;
        }
        else {
            alpha = -90;
        }
    }
    
    *alphaList = alpha;
    *(alphaList + 1) = alphaSin;
    *(alphaList + 2) = alphaCos;
         
    return alphaList;
}
