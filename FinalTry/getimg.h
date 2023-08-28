#ifndef GETIMG_H
#define GETIMG_H
#include"opencv2/opencv.hpp"
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include"qdatetime.h"
#include "function.h"
cv::Mat compress(cv::Mat img[]);
void getimg();
#endif // GETIMG_H
