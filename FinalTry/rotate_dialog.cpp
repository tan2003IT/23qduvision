#include "rotate_dialog.h"
#include "ui_rotate_dialog.h"

rotate_dialog::rotate_dialog(QImage &r,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rotate_dialog)
{
    ui->setupUi(this);

    //让lineedit和slider匹配
    QLineEdit *lineEdit = ui->lineEdit;
    QSlider *slider = ui->Slider;
    connect(lineEdit,&QLineEdit::textChanged,slider,[slider,lineEdit](){
       bool ok;
       int value = lineEdit->text().toInt(&ok);
       if(ok){
           slider->setValue(value);
       }
    });
    connect(slider,&QSlider::valueChanged,lineEdit,[lineEdit,slider](){
       QString text = QString::number(slider->value());
       lineEdit->setText(text);
    });


    // 连接 QSlider 控件的 valueChanged() 信号到一个槽函数中
    connect(slider, &QSlider::valueChanged, this, [=](){
        rotate_init = r;
        // 计算旋转角度并旋转图片
        QPixmap pixmap = QPixmap::fromImage(r);
        //r为引用，但是转为xpixmap之后的旋转不会影响r
        QTransform transform;
        transform.rotate(slider->value());
        pixmap = pixmap.transformed(transform, Qt::SmoothTransformation);
//        qDebug()<<ui->label_6->width()<<ui->label_6->height();
        ui->label_6->setFixedSize(337,236);
        ui->label_6->setScaledContents(true);
        ui->label_6->setPixmap(pixmap);
        rotate_image = pixmap.toImage();
    });
}

rotate_dialog::~rotate_dialog()
{
    delete ui;
}

int rotate_dialog::getAngle(){
    return angle;
}

void rotate_dialog::on_buttonBox_2_accepted()
{
    angle = ui->Slider->value();
    close();
}

void rotate_dialog::on_buttonBox_2_rejected()
{
    angle = 0;
    close();
}
