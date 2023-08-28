#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "function.h"
//#include"getdistence.cpp"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /*还没找到合适的图，先不要背景了*/
//    QPixmap background(":///back1.png"); // 加载图像资源
//    background = background.scaled(QSize(3071,1920)); // 屏幕尺寸
//    QPalette palette;
//    palette.setBrush(QPalette::Background, background); // 使用 QPalette 将图像设置为背景
//    this->setPalette(palette); // 将 QPalette 应用到主窗口

    ui->setupUi(this);

    //根据时间的路径
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::time_upadate);//注意和qt4.0之前版本区分开
    timer->start(1000);

    //撤回和恢复的的置空
    step.clear();
    recover.clear();


    //双线程尝试

    thread = new QThread(this);
    connect(thread, &QThread::started, []() {
       getimg();
    });
    thread->start();

//    thread2 = new QThread(this);
//    connect(thread2, &QThread::started, [this,cv::Mat]() {
//       face_detect();
//    });
//    thread2->start();


    //注意QPushButton和QAction的信号不一样
    connect(ui->action_open,&QAction::triggered,this,&MainWindow::on_open_btn_clicked);
    connect(ui->actionsave,&QAction::triggered,this,&MainWindow::on_save_btn_clicked);
    connect(ui->actionreset,&QAction::triggered,this,&MainWindow::on_reset_btn_clicked);
    connect(ui->actionrename,&QAction::triggered,this,&MainWindow::on_rename_btn_clicked);
    connect(ui->actionWarn,&QAction::triggered,this,&MainWindow::on_warn_btn_clicked);
    connect(ui->actioncheck,&QAction::triggered,this,&MainWindow::on_check_btn_clicked);
    connect(ui->action_cvt,&QAction::triggered,this,&MainWindow::on_cvt_btn_clicked);
    connect(ui->actionrotate,&QAction::triggered,this,&MainWindow::on_rotate_btn_clicked);
    connect(ui->action_fl,&QAction::triggered,this,&MainWindow::on_ud_rev_btn_clicked);
    connect(ui->action_rl,&QAction::triggered,this,&MainWindow::on_lr_rev_btn_clicked);
    connect(ui->action_ctrlz,&QAction::triggered,this,&MainWindow::on_ctrlZ_btn_clicked);
    connect(ui->action_ctrlY,&QAction::triggered,this,&MainWindow::on_ctrlY_btn_clicked);
    connect(ui->action90,&QAction::triggered,this,&MainWindow::on_rotate90_btn_clicked);
    connect(ui->action270,&QAction::triggered,this,&MainWindow::on_rotate270_btn_clicked);
    connect(ui->action_help,&QAction::triggered,this,&MainWindow::on_help_btn_clicked);

    connect(ui->help_btn, &QPushButton::clicked, this, &MainWindow::on_help_btn_clicked);


    //tool bar部分
    m_toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea,m_toolBar);//创建工具栏
    m_openAction = ui->action_open;
    m_saveAction = ui->actionsave;
    m_warnAction = ui->actionWarn;
    m_chekAction = ui->actioncheck;
    m_cvtAction = ui->action_cvt;
    m_rotateAction = ui->actionrotate;
    m_rotate90Action = ui->action90;
    m_rotate270Action = ui->action270;
    m_ctrlYAction = ui->action_ctrlY;
    m_ctrlZAction = ui->action_ctrlz;
    m_udAction = ui->action_fl;
    m_rlAction = ui->action_rl;
    m_helpAction = ui->action_help;
    m_toolBar->addAction(m_openAction);//open功能添加到toolBar
    m_toolBar->addAction(m_saveAction);//save功能添加到toolBar
    m_toolBar->addAction(m_warnAction);
    m_toolBar->addAction(m_chekAction);
    m_toolBar->addAction(m_cvtAction);
    m_toolBar->addAction(m_rotateAction);
    m_toolBar->addAction(m_rotate90Action);
    m_toolBar->addAction(m_rotate270Action);
    m_toolBar->addAction(m_ctrlZAction);
    m_toolBar->addAction(m_ctrlYAction);
    m_toolBar->addAction(m_udAction);
    m_toolBar->addAction(m_rlAction);
    m_toolBar->addAction(m_helpAction);
}

MainWindow::~MainWindow()
{
//    delete thread;

//    if (thread2 != nullptr && thread2->isRunning()) {
//        // 线程不为空且正在运行
//        delete thread2;
//    }

    if (thread != nullptr && thread->isRunning()) {
        // 线程不为空且正在运行
        delete thread;
    }

    delete ui;

}




void MainWindow::time_upadate(){
    QDateTime time = QDateTime::currentDateTime();
    video_filename = "E:\\22qduvision\\marphoto\\" + time.addSecs(-2).toString("yyyy_MM_dd_hh_mm_ss") + ".png";
//    qDebug()<<video_filename;
    video.load(video_filename);
//    img_tmp.load(video_filename);

    //针对全屏的尺寸大小
    ui->video_label->setFixedSize(175, 128);
    ui->img1->setFixedSize(153, 118);
    ui->img2->setFixedSize(153, 118);
    ui->img3->setFixedSize(153, 118);
    ui->img4->setFixedSize(153, 118);
    ui->img5->setFixedSize(153, 118);

    // 使 QLabel 控件自适应缩放，保持宽高比不变
    ui->video_label->setScaledContents(true);
    ui->img1->setScaledContents(true);
    ui->img2->setScaledContents(true);
    ui->img3->setScaledContents(true);
    ui->img4->setScaledContents(true);
    ui->img5->setScaledContents(true);

//    qDebug()<<ui->main_img->width()<<ui->main_img->height();

    ui->video_label->setPixmap(QPixmap::fromImage(video));


    for(int i=4;i>0;--i){
        past[i]=past[i-1];
        switch(i){
        case 4:ui->img5->setPixmap(QPixmap::fromImage(past[4]));break;
        case 3:ui->img4->setPixmap(QPixmap::fromImage(past[3]));break;
        case 2:ui->img3->setPixmap(QPixmap::fromImage(past[2]));break;
        case 1:ui->img2->setPixmap(QPixmap::fromImage(past[1]));break;
        }
    }
    past[0]=video;
    ui->img1->setPixmap(QPixmap::fromImage(past[0]));
//    qDebug()<<video_filename<<endl;
}

void MainWindow::on_warn_btn_clicked()
{
    //记得在.pro里加上 QT += multimedia

    QMediaPlaylist *playlist;
    QMediaPlayer *player;
    //循环播放音频
//    QString currentPath = QDir::currentPath(); // 获取当前工作目录
//    QDir dir(currentPath); // 构建当前目录对应的 QDir 对象
//    qDebug()<<dir;
//    QString par = dir.absolutePath()+"/Resources/ALARM.wav";
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl::fromLocalFile("E:/22qduvision/qdu_plusplus/FinalTry/ALARM.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    player = new QMediaPlayer();
    player->setPlaylist(playlist);
//    qint64 t = 1000;
//    player->setPosition(t);
    // 播放音频文件
    player->play();


    //弹出小窗，等待工作人员确认
    QMessageBox msgBox;
    msgBox.setWindowTitle("Alarm");
//    msgBox.setMinimumSize(110,110);
    msgBox.setText("\n\nsomething is wrong, please check it!!!");
    QPixmap pixmap(":/alarm_icon.png"); // 图标文件路径
    pixmap = pixmap.scaled(100,100);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok) {
        player->stop();
    }
}

void MainWindow::on_main_btn_clicked()
{
    if(flag){
        //说明有未保存的图，弹出窗口询问
        if(!check_img_save()) return ;
    }

    //撤回和恢复的的置空
    step.clear();
    recover.clear();

    flag = 0;
    image_init = video;
    image = video;

    step.push(image);

    ui->main_img->setFixedSize(1442 ,788);
    ui->main_img->setPixmap(QPixmap::fromImage(video));
}

void MainWindow::on_open_btn_clicked()
{
    if(flag){
        //说明有未保存的图，弹出窗口询问
        if(!check_img_save()) return ;
    }
    ui->main_img->setFixedSize(1442 ,788);
    //用文件夹打开
    QString filename_img = QFileDialog::getOpenFileName(this, tr("open img"),"E:/22qduvision/marphoto",tr("Image Files (*.png *.jpg *.bmp)"));//""中间放的是指定的目录
    if(!filename_img.isEmpty()){
        image_init.load(filename_img);
        image.load(filename_img);//所有类型的图片都建议用Qimage打开,为了防止覆盖图片的时候重复创建image（这样容易内存泄漏），只创建一个，之后更改地址就好
        ui->main_img->setPixmap(QPixmap::fromImage(image));//Qlabel中提供了setPixmap,但没有setimage，所以需要转换
    }

    //撤回和恢复的的置空
    step.clear();
    recover.clear();

    flag = 0;

    step.push(image);

    ui->main_img->setPixmap(QPixmap::fromImage(image));
    ui->main_img->setScaledContents(true);
}

void MainWindow::on_show_init_btn_pressed()
{
    ui->main_img->setPixmap(QPixmap::fromImage(image_init));//长按展示原图
}

void MainWindow::on_show_init_btn_released()
{
    ui->main_img->setPixmap(QPixmap::fromImage(image));//释放就展示之前的图片
}

void MainWindow::on_rename_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    //保存init_image
    QString savePath = QFileDialog::getSaveFileName(this, "图片保存", "", "Image Files (*.png *.jpg *.bmp)");
    if(savePath.isEmpty()) return ;
    image_init.save(savePath);
}

void MainWindow::on_check_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    //不能很好的检查，要不就直接显示图片信息，分辨率，+rgb/argb
    QString format;
    int width=image_init.width(),length=image_init.height();
    QString  w = "\nwidth:"+QString::number(width) + "  length:"+QString::number(length);
    switch (image_init.format()) {
        case QImage::Format_Mono:
            format+="format:Mono\n"+w;
            break;
        case QImage::Format_MonoLSB:
            format+="format:MonoLSB\n"+w;
            break;
        case QImage::Format_Indexed8:
            format+="format:Indexed8\n"+w;
            break;
        case QImage::Format_RGB32:
            format+="format:RGB32\n"+w;
            break;
        case QImage::Format_ARGB32:
            format+="format:ARGB32\n"+w;
            break;
        case QImage::Format_ARGB32_Premultiplied:
            format+="format:ARGB32_Premultiplied\n"+w;
            break;
        case QImage::Format_RGB16:
            format+="format:RGB16\n"+w;
            break;
        case QImage::Format_ARGB8565_Premultiplied:
            format+="format:ARGB8565_Premultiplied\n"+w;
            break;
        case QImage::Format_RGB666:
            format+="format:RGB666\n"+w;
            break;
        case QImage::Format_ARGB6666_Premultiplied:
            format+="format:ARGB6666_Premultiplied\n"+w;
            break;
        case QImage::Format_RGB555:
            format+="format:RGB555\n"+w;
            break;
        case QImage::Format_ARGB8555_Premultiplied:
            format+="format:ARGB8555_Premultiplied\n"+w;
            break;
        case QImage::Format_RGB888:
            format+="format:RGB888\n"+w;
            break;
        case QImage::Format_RGB444:
            format+="format:RGB444\n"+w;
            break;
        case QImage::Format_ARGB4444_Premultiplied:
            format+="format:ARGB4444_Premultiplied\n"+w;
            break;
        default:
            format+="Unknown image format";
        break;}
    QMessageBox::information(this, "Image ", format);
}

void MainWindow::on_save_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    QString savePath = QFileDialog::getSaveFileName(this, "图片保存", "", "Image Files (*.png *.jpg *.bmp)");
    if(savePath.isEmpty()) return ;
    image.save(savePath);
    flag = 0;
}

void MainWindow::on_cvt_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    image = image.convertToFormat(QImage::Format_Grayscale8);
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;
}

void MainWindow::on_rotate_btn_clicked()
{

//    thread->start();

    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    rotate_dialog * pdialog;
    rotate_dialog rDialog(image);
    pdialog = &rDialog;
    rDialog.exec();
    int angle = pdialog->getAngle();
    image = image.transformed(QTransform().rotate(angle));
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;
}

void MainWindow::on_ud_rev_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    image = image.mirrored(false,true);
    ui->main_img->setPixmap(QPixmap::fromImage(image));
    step.push(image);
    flag = 1;

}

void MainWindow::on_lr_rev_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    image = image.mirrored(true,false);
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;

}

void MainWindow::on_rotate90_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    image = image.transformed(QTransform().rotate(90));
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;
}

void MainWindow::on_rotate270_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    image = image.transformed(QTransform().rotate(270));
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;
}

void MainWindow::on_reset_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    image = image_init;
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;

}

void MainWindow::on_ctrlZ_btn_clicked()
{
    //查看是否可撤销
    if(step.size()<=1){//这里是因为如果只是当前不空，pop之后可能空，导致top异常
        QMessageBox msgBox;
        msgBox.setText("\n\n已无可撤销操作！！！");
        QPixmap pixmap(":/save_or_not.png"); // 图标文件路径
        pixmap = pixmap.scaled(100,100);
        msgBox.setIconPixmap(pixmap);
        msgBox.exec();
        return ;
    }else{
        recover.push(step.pop());
        image = step.top();
        ui->main_img->setPixmap(QPixmap::fromImage(image));
        flag = 1;
    }


}

void MainWindow::on_ctrlY_btn_clicked()
{
    //查看是否可恢复
    if(recover.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("\n\n已无可恢复操作！！！");
        QPixmap pixmap(":/save_or_not.png"); // 图标文件路径
        pixmap = pixmap.scaled(100,100);
        msgBox.setIconPixmap(pixmap);
        msgBox.exec();
        return ;
    }else{

        image = recover.top();
        recover.pop();
        step.push(image);
        ui->main_img->setPixmap(QPixmap::fromImage(image));
        flag = 1;
    }
}



void MainWindow::on_ex_btn_clicked()
{

    if(flag){
        //说明有未保存的图，弹出窗口询问
        //说明有未保存的图，弹出窗口询问
        QMessageBox msgBox;
        msgBox.setWindowTitle("您未保存文档！");
        msgBox.setText("\n\n您未保存文档，确定要退出吗？？？");
        QPixmap pixmap(":/save_or_not.png"); // 图标文件路径
        pixmap = pixmap.scaled(100,100);
        msgBox.setIconPixmap(pixmap);
        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok) exit(0);
        else return;

    }

    exit(0);
}

void MainWindow::on_correct_btn_clicked()
{

    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }

    //直接弹框：该图像无法校正
    QMessageBox msgBox;
    msgBox.setWindowTitle("该图像无法校正！");
    msgBox.setText("\n\n该图像无法矫正！！！");
    QPixmap pixmap(":/save_or_not.png"); // 图标文件路径
    pixmap = pixmap.scaled(100,100);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();

   cv::Mat tmp;
   cv::Mat tt = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine()).clone() ;
//   cv::imshow("kjkl",tt);
//   tmp=main_drawimg(tt);
//   ui->main_img->setPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_help_btn_clicked()
{

    h.show();
}

void MainWindow::on_dis_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
//    if(image.isNull()){
//        check_img_Null();
//        return ;
//    }

   cv::Mat tmp;
   cv::Mat tt = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine()).clone() ;
   //getDistance(tt);
   getdistence();
   cv::imshow(" ",tmp);
}

void MainWindow::on_face_btn_clicked()
{
    //查看iamge是否为空，为空提示禁止使用
    if(image.isNull()){
        check_img_Null();
        return ;
    }
//    cv::Mat tmp;
    cv::Mat tt = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine()).clone() ;
    face_detect(tt);
    cv::cvtColor(tt, tt, cv::COLOR_BGR2RGB);
    image = QImage(tt.data, tt.cols, tt.rows, static_cast<int>(tt.step), QImage::Format_RGB888);
    ui->main_img->setPixmap(QPixmap::fromImage(image));

    step.push(image);
    flag = 1;

//    cv::imshow("i",tt);
}
