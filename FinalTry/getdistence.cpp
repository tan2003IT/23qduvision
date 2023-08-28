#include"getdistence.h"

using namespace cv;
using namespace std;
//2903.85   2438.65   2904.1
#define KNOWN_DISTANCE  70.866		//已知距离
#define KNOWN_WIDTH   0.787		//已知宽度0.787    11.69
#define KNOWN_HEIGHT  0.787	//已知高度0.787    8.27
//需要提前获取,通过标定步骤获得的
#define KNOWN_FOCAL_LENGTH  3000  //已知焦距

double MaxDistance= DBL_MIN;//最大距离
double MinDistance= DBL_MAX;//最小距离
void GetPicture(Mat& SrcImage, int choice);
void GetCamera(int choice);
//void GetVideo(int choice);
double GetTheDistanceToCamera(double KnownWidth, double FocalLength, double PerWidth);
double CalculateFocalDistance(Mat& Image);
RotatedRect FindMarker(Mat& SrcImage);

void GetPicture(Mat& SrcImage,int choice) {

    //以下程序用于标定相机获得“焦距”
    // namedWindow("输入窗口", 0);
    // resizeWindow("输入窗口", 600, 600);//限定窗口大小
    // imshow("输入窗口", SrcImage);//输出窗口
    double FocalLength = 0.0;
    FocalLength = CalculateFocalDistance(SrcImage);  //获得焦距

    //以下程序用于实际计算距离
    RotatedRect Marker;
    Marker = FindMarker(SrcImage);
    double DistanceInches = 0.0;

    DistanceInches = GetTheDistanceToCamera(KNOWN_WIDTH, KNOWN_FOCAL_LENGTH, Marker.size.width); //计算相机与目标之间的距离
    DistanceInches = DistanceInches * 2.54;  //转换成cm  1英寸=2.54厘米
    //获取检测到的最大最小距离的范围

    if (DistanceInches > MaxDistance)
        MaxDistance = DistanceInches;
    if (DistanceInches < MinDistance)
        MinDistance = DistanceInches;
    cout << "DistanceInches(cm):" << DistanceInches << endl;  //显示的单位为厘米
    putText(SrcImage, format("distance(cm):%f", DistanceInches), Point(100, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255,0), 3, LINE_8);//在图片上显示文本
    namedWindow("输出窗口", 0);
    resizeWindow("输出窗口", 600, 600);
    imshow("输出窗口", SrcImage);
    if (choice != 1) { //如果检测视频或者摄像头，就延时1ms，如果检测图片，就停留在当前帧
        waitKey(1);
    }
    else if (choice == 1) {
        waitKey(0);
    }
}


void getdistence()
{
    int choice = 0;


    choice = 2;
    if (choice == 1 ){
//        Mat SrcImage = imread("E:\\22qduvision\\marphoto\\2023_06_15_15_13_41.png", IMREAD_COLOR);//获取图片
//        GetPicture(SrcImage,choice);
//        cout << "图像中检测过的轮廓中，最大距离为：" << MaxDistance << "cm" << endl;
//        cout << "图像中检测过的轮廓中，最小距离为：" << MinDistance << "cm" << endl;
    }
    else if (choice == 2) {
        GetCamera(choice);
    }
//    else if (choice == 3) {
//        GetVideo(choice);
   // }

}


void GetCamera(int choice) {

    Mat frame;
    VideoCapture capture(0);//读取视摄像头实时画面数据，0默认是笔记本的摄像头；如果是外接摄像头，这里改为1

    while (true)
    {
        capture >> frame;            //读取当前帧
        GetPicture(frame,choice);
        int key = waitKey(10);
        if (key == 32) {
            break;
            destroyAllWindows();   //释放全部窗口
        }

    }
    cout << "图像中检测过的轮廓中，最大距离为：" << MaxDistance << "cm" << endl;
    cout << "图像中检测过的轮廓中，最小距离为：" << MinDistance << "cm" << endl;
    //capture.release();     //释放摄像头资源
//    destroyAllWindows();   //释放全部窗口

}

//void GetVideo(int choice) {
//    VideoCapture capture("视频的绝对路径.mp4");

//    Mat frame;

//    if (capture.isOpened())  //判断视频是否成功打开
//    {
//        //capture.grab() 从视频文件或捕获设备中抓取下一个帧
//        while (capture.grab()) {
//            capture >> frame;
//            //GetPicture(frame,choice);
//            waitKey(1);
//            /*int key = waitKey(10);
//            if (key == 32) {
//                waitKey(0);
//            }
//            if (key == 27) {
//                break;
//            }*/
//        }

//    }
//    cout << "图像中最大距离为：" << MaxDistance << "cm" << endl;
//    cout << "图像中最小距离为：" << MinDistance << "cm" << endl;
//    waitKey();
//}

double GetTheDistanceToCamera(double KnownWidth, double FocalLength, double PerWidth)
{
    return (KnownWidth * FocalLength) / PerWidth; //计算目标到相机的距离   KnownWidth-为已知的目标的宽度  FocalLength-焦距   PerWidth-图像中宽度的像素数
}

RotatedRect FindMarker(Mat& SrcImage)//画出图形的边界并返回最小外接矩形
{
    Mat GrayImage;
    cvtColor(SrcImage, GrayImage, COLOR_BGR2GRAY);//将SrcImage复制给GrayImage

    Mat GaussImage;
    //将GrayImage通过高斯滤波处理后存放到GaussImage中
    GaussianBlur(GrayImage, GaussImage, Size(3, 7), 3); //高斯滤波，对图像进行滤波操作（平滑操作、过滤操作，去噪操作）

    Mat EdgeImage;
    Canny(GaussImage, EdgeImage, 100, 200);//边缘检测

/*这段代码可省略，只是用来看效果。
    namedWindow("复制图", 0);
    resizeWindow("复制图", 600, 600);
    imshow("复制图", GrayImage);
    namedWindow("高斯滤波处理", 0);
    resizeWindow("高斯滤波处理", 600, 600);
    imshow("高斯滤波处理", GaussImage);
    namedWindow("边缘检测处理", 0);
    resizeWindow("边缘检测处理", 600, 600);
    imshow("边缘检测处理", EdgeImage);
*/
    vector<vector<Point>> Contours;
    vector<Vec4i> Hierarchy;
    findContours(EdgeImage.clone(), Contours, Hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE /*,Point(10,20)*/);//检测物体轮廓
    double Area = -1;
    int Index = -1;
    for (int i = 0; i < Contours.size(); i++)//得到最大的轮廓然后画出来，其实是一个点集
    {
        if (contourArea(Contours[i]) > Area)
        {
            Area = contourArea(Contours[i]);
            Index = i;
        }
    }
    //第三个参数为-1的时候就画出全部轮廓
    drawContours(SrcImage, Contours, -1, Scalar(0, 0, 255), 10, LINE_8);//画出物体的轮廓

    RotatedRect Rorect;
    Rorect = minAreaRect(Contours[Index]);  //得到被测物的最小外接矩形
    // 不提供2.0版本
    return Rorect;
}

double CalculateFocalDistance(Mat& Image)//计算拍照相机的焦距
{
    RotatedRect Marker;
    Marker = FindMarker(Image);
    double FocalLength = 0.0;
    double FocalWide = 0.0;
    FocalLength = (Marker.size.height * KNOWN_DISTANCE) / KNOWN_WIDTH;  //计算焦距单位为图像像素  依据公式： F=(P*D)/W   F-焦距   D-距离   P-像素宽度   W-目标的真实宽度（单位英寸）
    FocalWide = (Marker.size.width * KNOWN_DISTANCE) / KNOWN_HEIGHT;
    cout << "标定焦距:" << FocalLength << endl;
    cout << "标定焦距:" << FocalLength << endl;
    return FocalLength;
    //1857.71
}
