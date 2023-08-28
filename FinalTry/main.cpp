#include "mainwindow.h"
#include "getdistence.cpp"
#include"opencv2/opencv.hpp"
#include"getimg.h"
#include"function.h"
#include <QApplication>
using namespace cv;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
