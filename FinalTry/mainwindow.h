#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "check.h"
#include "getdistence.h"
#include "rotate_dialog.h"
#include "help_dialog.h"
#include "getimg.h"
#include "function.h"
#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QMessageBox>
#include <QStack>
#include <QFileDialog>
#include <QToolBar>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void time_upadate();

private slots:
    void on_warn_btn_clicked();

    void on_main_btn_clicked();

    void on_open_btn_clicked();

    void on_show_init_btn_pressed();

    void on_show_init_btn_released();

    void on_rename_btn_clicked();

    void on_check_btn_clicked();

    void on_save_btn_clicked();

    void on_cvt_btn_clicked();

    void on_rotate_btn_clicked();

    void on_ud_rev_btn_clicked();

    void on_lr_rev_btn_clicked();

    void on_rotate90_btn_clicked();

    void on_rotate270_btn_clicked();

    void on_reset_btn_clicked();

    void on_ctrlZ_btn_clicked();

    void on_ctrlY_btn_clicked();

    void on_ex_btn_clicked();

    void on_correct_btn_clicked();

    void on_help_btn_clicked();

    void on_dis_btn_clicked();

    void on_face_btn_clicked();

private:
    Ui::MainWindow *ui;

    Help_Dialog h;

    //线程
    QThread *thread;
//    , *thread2;

    //图像
    QImage image_init,img_tmp, image,video,past[5];

    //撤回和恢复
    QStack<QImage> step,recover;

    //路径
    QString video_filename,img_filename;

    bool flag=0;//当前main——label中是否存在未保存的图

    //tool bar
    QToolBar *m_toolBar;
    QAction *m_openAction, * m_saveAction, * m_warnAction, * m_chekAction, * m_cvtAction, * m_rotateAction;
    QAction *m_rotate90Action, * m_rotate270Action, *m_ctrlYAction, *m_ctrlZAction, *m_udAction, *m_rlAction, *m_helpAction;

};
#endif // MAINWINDOW_H
