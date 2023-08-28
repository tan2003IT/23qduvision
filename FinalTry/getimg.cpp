#include "getimg.h"
#include "function.h"    //主要是为了同时对异常图像进行自动处理
#include "iostream"
using namespace std;
using namespace cv;
void getimg(){
    // 打开默认摄像头
        VideoCapture cap(0);
        if (!cap.isOpened()) {
            cerr << "Failed to open camera" << endl;
        }

        // 进行视频捕获和处理
        Mat frame;
        int cnt = 0;
        Mat video[8];
        QString filename1;
        string filename;
        QString abnormalname1;
        string abnormalname;
        while (true) {
            cap.read(frame);
            if (frame.empty()) break;
            video[cnt] = frame;
            if (cnt == 7) {
                //调用函数
                cnt = 0;
                QDateTime time = QDateTime::currentDateTime();
                filename1 = "E:\\22qduvision\\marphoto\\" + time.toString("yyyy_MM_dd_hh_mm_ss") + ".png";  //QT库中有
                filename = filename1.toStdString();
                Mat dst = compress(video);   //对图片进行压缩
                abnormalname1 = "E:\\22qduvision\\abnormalmarphoto\\" + time.toString("yyyy_MM_dd_hh_mm_ss") + ".png";
                abnormalname = abnormalname1.toStdString();
                //接下来对进行异常判断与处理
                itemSearch(dst,abnormalname);  //将异常图片保存到一个特定文件夹里

                puttime(dst);     //图片标注时间
//                face_detect(dst);
                imwrite(filename,dst);

            }
            else cnt++;
            //imshow("Video", frame);
            waitKey(120);
//            if (waitKey(125) == 'q') break;  // 按下 Q 键退出
        }

        // 释放资源并退出
        cap.release();
}
