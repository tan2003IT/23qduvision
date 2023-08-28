#include "function.h"
#include"iostream"
#include"string"
#include"opencv2/imgproc/imgproc_c.h"
#include"ctime"

////以下是测距所需要的数据
////2903.85   2438.65   2904.1
//#define KNOWN_DISTANCE  70.866		//已知距离
//#define KNOWN_WIDTH   0.787		//已知宽度0.787    11.69
//#define KNOWN_HEIGHT  0.787	//已知高度0.787    8.27
////需要提前获取,通过标定步骤获得的
//#define KNOWN_FOCAL_LENGTH  3000  //已知焦距



using namespace std;
using namespace cv;



//自动校正图像

// comparison function object
bool cmp (std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
    double i = fabs(contourArea(cv::Mat(contour1)));
    double j = fabs(contourArea(cv::Mat(contour2)));
    return (i < j);
}

cv::Mat Get_outline(cv::Mat& img){
    Mat grayimg,blurimg,edgeimg;
    cvtColor(img,grayimg,COLOR_BGR2GRAY);
    threshold(grayimg,grayimg,85,255,THRESH_BINARY);
    //GaussianBlur(grayimg,blurimg,Size(5,5),0);
    Canny(blurimg,edgeimg,100,50);
    return edgeimg;
}
std::vector<cv::Point> Get_cnt(cv::Mat &img){
    //获得四角点
    vector<vector<Point>>contours;
    vector<Vec4i>hierarchy;
    findContours(img,contours,hierarchy,CV_RETR_EXTERNAL,CHAIN_APPROX_NONE);//只检测最外层轮廓
    /*接下来对所有所获得轮廓进行根据面积由大到小进行排序，通过approPolyPC()函数获取近似轮廓，
      当某个轮廓只有四个顶点时，获取这四个顶点，退出循环，返回这组向量*/
    if(!contours.empty()){   //如果vector不为空，即存在轮廓
        //根据面积进行排序
        sort(contours.begin(),contours.end(),cmp);
        for(auto c :contours){
            //先对具体的某个轮廓求长度：
            double length = arcLength(c,true);
            vector<Point> approx_contours;
            approxPolyDP(c,approx_contours,0.02*length,true);
            if(approx_contours.size()==4){
                return approx_contours;
            }
        }
    }
}
cv::Mat main_drawimg(cv::Mat &img){
    Mat edgeimg = Get_outline(img);
    vector<Point> docCnt = Get_cnt(edgeimg);
    Point2f src[4];
    for(int i = 0;i<4;i++){
        src[i] = docCnt[i];
    }
    Point2f dst[4] = {Point2f(0,0),Point2f(380,0),Point2f(0,360),Point2f(380,360)};
    Mat dstimg,transimage = getPerspectiveTransform(src,dst);
    //接下来就是 warpPerspective()
    warpAffine(img,dstimg,transimage,Size(380,360));
    return dstimg;
}

//以上为图像自动校正 其实在 itemSearch 里面已经实现了

////以下是单目测距
//double MaxDistance= DBL_MIN;//最大距离
//double MinDistance= DBL_MAX;//最小距离

//double CalculateFocalDistance(Mat& Image);
//double GetTheDistanceToCamera(double KnownWidth, double FocalLength, double PerWidth)
//{
//    return (KnownWidth * FocalLength) / PerWidth; //计算目标到相机的距离   KnownWidth-为已知的目标的宽度  FocalLength-焦距   PerWidth-图像中宽度的像素数
//}

//RotatedRect FindMarker(Mat& SrcImage){

//    Mat GrayImage;
//    cvtColor(SrcImage, GrayImage, COLOR_BGR2GRAY);//将SrcImage复制给GrayImage

//    Mat GaussImage;
//    //将GrayImage通过高斯滤波处理后存放到GaussImage中
//    GaussianBlur(GrayImage, GaussImage, Size(3, 7), 3); //高斯滤波，对图像进行滤波操作（平滑操作、过滤操作，去噪操作）

//    Mat EdgeImage;
//    Canny(GaussImage, EdgeImage, 100, 200);//边缘检测

///*这段代码可省略，只是用来看效果。
//    namedWindow("复制图", 0);
//    resizeWindow("复制图", 600, 600);
//    imshow("复制图", GrayImage);
//    namedWindow("高斯滤波处理", 0);
//    resizeWindow("高斯滤波处理", 600, 600);
//    imshow("高斯滤波处理", GaussImage);
//    namedWindow("边缘检测处理", 0);
//    resizeWindow("边缘检测处理", 600, 600);
//    imshow("边缘检测处理", EdgeImage);
//*/
//    vector<vector<Point>> Contours;
//    vector<Vec4i> Hierarchy;
//    findContours(EdgeImage.clone(), Contours, Hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE /*,Point(10,20)*/);//检测物体轮廓
//    double Area = -1;
//    int Index = -1;
//    for (int i = 0; i < Contours.size(); i++)//得到最大的轮廓然后画出来，其实是一个点集
//    {
//        if (contourArea(Contours[i]) > Area)
//        {
//            Area = contourArea(Contours[i]);
//            Index = i;
//        }
//    }
//    //第三个参数为-1的时候就画出全部轮廓
//    drawContours(SrcImage, Contours, -1, Scalar(0, 0, 255), 10, LINE_8);//画出物体的轮廓

//    RotatedRect Rorect;
//    Rorect = minAreaRect(Contours[Index]);  //得到被测物的最小外接矩形
//    return Rorect;
//}

////计算焦距：
//double CalculateFocalDistance(Mat& Image)//计算拍照相机的焦距
//{
//    RotatedRect Marker;
//    Marker = FindMarker(Image);
//    double FocalLength = 0.0;
//    double FocalWide = 0.0;
//    FocalLength = (Marker.size.height * KNOWN_DISTANCE) / KNOWN_WIDTH;  //计算焦距单位为图像像素  依据公式： F=(P*D)/W   F-焦距   D-距离   P-像素宽度   W-目标的真实宽度（单位英寸）
//    FocalWide = (Marker.size.width * KNOWN_DISTANCE) / KNOWN_HEIGHT;
//    cout << "标定焦距:" << FocalLength << endl;
//    cout << "标定焦距:" << FocalLength << endl;
//    return FocalLength;
//    //1857.71
//}




//void getDistance(Mat &img){
////    namedWindow("输入窗口", 0);
////        resizeWindow("输入窗口", 600, 600);//限定窗口大小
////        imshow("输入窗口", img);//输出窗口
//    //获取焦距
//    double FocalLength = 0.0;
//    FocalLength = CalculateFocalDistance(img);  //获得焦距

//    //以下程序用于实际计算距离
//    RotatedRect Marker;
//    Marker = FindMarker(img);
//    double DistanceInches = 0.0;
//    DistanceInches = GetTheDistanceToCamera(KNOWN_WIDTH, FocalLength, Marker.size.width); //计算相机与目标之间的距离
//    DistanceInches = DistanceInches * 2.54;  //转换成cm  1英寸=2.54厘米

//    //获取检测到的最大最小距离的范围
//    if (DistanceInches > MaxDistance)
//        MaxDistance = DistanceInches;
//    if (DistanceInches < MinDistance)
//        MinDistance = DistanceInches;
//    //cout << "DistanceInches(cm):" << DistanceInches << endl;  //显示的单位为厘米

//    //show distance:
//    putText(img, format("distance:%fcm", DistanceInches), Point(100, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 0, 0), 3, LINE_8);//在图片上显示文本
//    namedWindow("distance", 0);
//    resizeWindow("distance", 600, 600);
//    imshow("distance", img);
//}
////end

//以下为sobel算子勾绘轮廓
void getSobel(Mat &src,Mat &dst){
    //首先传入某一帧的图片src，对其进行降噪处理：
    Mat tmpimg,imgx,imgy;
    GaussianBlur(src,tmpimg,Size(3,3),0,0,BORDER_DEFAULT);
    //转灰度
    cvtColor(tmpimg,tmpimg,COLOR_RGB2GRAY);
    Scharr(tmpimg,imgx,CV_16S,1,0);
    Scharr(tmpimg,imgy,CV_16S,0,1);
    //接下来将上面两个分别对x，y求导的两个图像合并
    addWeighted(imgx,0.5,imgy,0.5,3,dst);
}


//以下为识别异常轮廓并勾绘
void itemSearch(Mat img,string name){
    //思路：利用inRange函数
        Mat hstimg;
        //二值化
        cvtColor(img,hstimg,COLOR_BGR2HSV);
        Mat mask;
        inRange(hstimg,Scalar(35,43,46),Scalar(77,255,255),mask); //二值化，识别green色，并将结果图像保存到mask中，mask为掩膜
        //通过bitwise_and获得想要的图像部分
        //Mat dst1;
        //bitwise_and(img,img,dst1,mask);//前两个参数一样，就是为了保持原图不动，在原图上操作
        //由于给出的图片中有电线的干扰，在dst1中为书本上的黑色区域，这里尝试利用腐蚀膨胀操作将电线消除
        //Mat dst2;
        //Mat kernel = getStructuringElement(MORPH_RECT,Size(10,10),Point(-1,-1));  //生成卷积核
        //morphologyEx(dst1,dst2,1,kernel);
        // for(int i = 0;i<6;i++){
        //     morphologyEx(dst2,dst2,1,kernel);  //多进行几次，不断将电线变细，直至不影响
        // }
        //接下来开始画出矩形边框   思路： 找到蓝色区域最大面积，用矩形框包围这个面积

        //在进行一次二值化
        //Mat dst3;
        //inRange(dst2,Scalar(100,43,46),Scalar(124,255,255),dst3);
        vector<vector<Point>>contours;
        vector<Vec4i> hierarchy;
        GaussianBlur(mask, mask, Size(3, 3), 0);//高斯滤波

        findContours(mask,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_NONE,Point());
        //drawContours(img, contours, -1, Scalar(255, 255, 255), 2);
        // //遍历轮廓显示矩形框
        //  Mat dst3 = img.clone();
        // vector<vector<Point>>approx_contours(contours.size());    //定义个与contours一样大的vector，用来存放逼近曲线的数组
        // for(int i = 0;i<contours.size();i++){
        //     approxPolyDP(contours[i],approx_contours[i],5,true);
        // }
        double max_area = 0;
        int maxnum = 0;
        //通过循环找出最大的方框，就是要求的图形的方框
        for (int i = 0; i < contours.size(); ++i)
        {
            if(max_area<contourArea(contours[i])){
                max_area = contourArea(contours[i]);
                maxnum = i;
            }
        }

        // cv::Rect rect = cv::boundingRect(Mat(contours[maxnum]));
        // //画出矩形框
        // cv::rectangle(img, rect, Scalar(0,0,255), 3);
        // //选中矩形框中的内容

        drawContours(img,contours,maxnum,Scalar(255,0,0),4);
        //接下来试试minAreaRect    实现报警（通过显示事物轮廓和设置最小矩形框框起来）
//        RotatedRect rect = minAreaRect(contours[maxnum]);
//            Point2f P[4];
//            rect.points(P);
//            vector<int> X_Contours;
//            for (int j = 0; j <= 3; j++)
//            {
//                // X_Contours.push_back(P[j].x);
//                // X_Contours.push_back(P[(j + 1) % 4].x);
//                line(img, P[j], P[(j + 1) % 4], Scalar(0,0, 255), 4);
//            }



//        //对矩形进行透视变换，并写入文件
//        Point2f src[4] = {P[2],P[3],P[1],P[0]};
//        Point2f dst[4] = {Point2f(0,0),Point2f(560,0),Point2f(0,360),Point2f(560,360)};
//        Mat M = getPerspectiveTransform(src,dst);
//        Mat fnimg;
//        warpPerspective(img,fnimg,M,Size(380,360));
        //最后保存这个fnimg即可 改一下名称
        if(max_area>500)   //如果轮廓大于一定面积，则保存图片
        imwrite(name,img);

}

//以下是图片压缩
//利用cvAddWeighted()实现对图像进行压缩，并通过平滑滤波对边界进行模糊处理，将目标内的小间断连接起来
Mat compress(Mat img[]){
    Mat dst = img[0];
    for(int i = 0;i<7;i++){
        addWeighted(dst,0.5,img[i],0.5,3,dst);
    }
    GaussianBlur(dst,dst,Size(3,3),0);   //高斯滤波
    return dst;
}

//图片设置时间
//想了想，利用火星日感觉并不太实际，难道火星还有时区？
//为了方便地球上的工作人员使用，仍采用了系统时间
void puttime(Mat &img){
    // 基于当前系统的当前日期/时间
    time_t now = time(0); // 把 now 转换为字符串形式
    string str = ctime(&now);
//    cout<<str;
    //设置文本相关系数
    int font_face = cv::FONT_HERSHEY_COMPLEX;
    double font_scale = 1;
    int thickness = 1;
    int baseline;
    //获取文本框长度
    Size textsize = getTextSize(str,font_face,font_scale,thickness,&baseline);
    //文本框右下角绘制
    Point origin;
    origin.x = img.cols - textsize.width+15;
    origin.y = img.rows - textsize.height + 13;
    putText(img,str,origin,font_face,font_scale,Scalar(0,255,0),thickness);

}


void face_detect(Mat &img){
    CascadeClassifier faceCascade;   //使用Viola-Jones算法来实人脸识别
    faceCascade.load("E:\\22qduvision\\qdu_plusplus\\FinalTry\\haarcascade_frontalface_default.xml");  //导入事先训练好的级联分类器

    if (faceCascade.empty()) { qDebug() << "XML file not loaded" << endl<<endl; }  //判断是否导入成功

    vector<Rect> faces;  //定义脸部显示框为矩形
    faceCascade.detectMultiScale(img, faces, 1.1, 10);  //将输入图像的脸部定位框位置上的四个节点输出

    for (int i = 0; i < faces.size(); i++)
    {
        rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);   //缩写top righy ; bottom left
    }

}
