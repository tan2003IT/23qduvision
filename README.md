# 23qduvision

**本项目是青岛大学2022级图灵班《程序设计和算法实践》一课的最终成品，根据“祝融号”火星探测器项目要求，写的一个图像处理工具。**   
   *该项目存在很多不完善的地方（例如导出.exe文件失败），仅供学习和交流，如若用于其他用途，请自行将这依托答辩修改，debug。*

---
请在使用前阅读如下提示！
+ 请用**QT打开FinalTry文件夹**，并确保您已安装opencv-c++，并version >= 3.5.2 (该项目用该版本编写，不清楚之前的能不能用)
+ 在使用时请先在FinalTry.pro文件中，将
  ~~~
  INCLUDEPATH += E:\opencv_vscode\vs\opencv\build\include
  LIBS += E:\opencv_vscode\vs\opencv\build\x64\MinGw\bin\libopencv_world452.dll
  ~~~
  更改为自己的路径
  ~~~
  INCLUDEPATH += ...
  LIBS += ...
  ~~~
+ 该项目会从电脑摄像头中读取图片并写入内存，请确保设备正常
+ 读取的图片会自动保存到文件夹中，请打开getimg.cpp文件并将`filename1 = "E:\\22qduvision\\marphoto\\" + time.toString("yyyy_MM_dd_hh_mm_ss") + ".png"; `中的`"E:\\22qduvision\\marphoto\\" `更改为自己创建的文件夹
+ 文件中`imread`和`imwrite`许多路径都是用绝对路径写的，读者在使用时应当注意，改为自己的相对路径

在将路径修改之后，这坨答辩就能运行了，祝您好运！😄

工具的具体使用说明在软件里有，同时也给出了docx版的README文件提供更详细的使用说明


*附赠：答辩PPT，仅供参考*
*链接：https://pan.baidu.com/s/1ZkmL69radr-pH6VjygCiig 
提取码：1111*
