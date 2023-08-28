#ifndef ROTATE_DIALOG_H
#define ROTATE_DIALOG_H

#include <QDialog>

namespace Ui {
class rotate_dialog;
}

class rotate_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit rotate_dialog(QImage &r,QWidget *parent = nullptr);
    ~rotate_dialog();
    int getAngle();

private slots:
    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

private:
    Ui::rotate_dialog *ui;
    QImage rotate_image,rotate_init;
    int angle = 0;
};

#endif // ROTATE_DIALOG_H
