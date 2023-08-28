#include "check.h"

void check_img_Null(){
        QMessageBox msgBox;
        msgBox.setWindowTitle("所处理图像为空！");
        msgBox.setText("\n\n您未选择要处理的图像，请选择后再处理！！！");
        QPixmap pixmap(":/save_or_not.png"); // 图标文件路径
        pixmap = pixmap.scaled(100,100);
        msgBox.setIconPixmap(pixmap);
        msgBox.exec();
        return ;

}

bool check_img_save(){
    //说明有未保存的图，弹出窗口询问
    QMessageBox msgBox;
    msgBox.setWindowTitle("您未保存文档！");
    msgBox.setText("\n\n您未保存文档，确定要对新图片进行处理吗？？？");
    QPixmap pixmap(":/save_or_not.png"); // 图标文件路径
    pixmap = pixmap.scaled(100,100);
    msgBox.setIconPixmap(pixmap);
    msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok) {
        return true;
    }
    return false;
}
