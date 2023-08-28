#ifndef FUNCTION_H
#define FUNCTION_H

#include"opencv2/opencv.hpp"
#include"vector"
#include <QDebug>

void getSobel(cv::Mat&,cv::Mat&);   //sobel边缘检测

void puttime(cv::Mat&);    //设置时间

//void getDistance(cv::Mat&);    //测距


cv::Mat compress(cv::Mat img[]);    //图片压缩

void itemSearch(cv::Mat img,std::string name);      //勾绘物体轮廓

//以下是获得仿射变换
cv::Mat Get_outline(cv::Mat&);
std::vector<cv::Point> Get_cnt(cv::Mat&);       //获得四角点     //没写函数名，我真sb
cv::Mat main_drawimg(cv::Mat &);      //将上面两个函数封装在这个函数里，直接在这个函数里实现
//如上

void face_detect(cv::Mat&);


#endif // FUNCTION_H
