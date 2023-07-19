#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QImage>
#include    <QPrinter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString m_filename;  //当前图片文件名
    QImage  m_image;          //原始图像

    void  showImageFeatures(bool formatChanged=true);  //显示图像属性
    void  imageModified(bool modified=true);    //图像被修改了，改变actions状态

    void  printImage(QPainter *painter, QPrinter *printer);   //打印图像
    void  printRGB565Data(QPainter *painter, QPrinter *printer);  //打印RGB565数据

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void do_paintRequestedImage(QPrinter *printer);     //用于打印图片

    void do_paintRequestedText(QPrinter *printer);      //用于打印文本


    void on_actFile_Open_triggered();

    void on_actImg_RotateLeft_triggered();

    void on_actImg_FlipUD_triggered();

    void on_actImg_FlipLR_triggered();

    void on_actImg_RotateRight_triggered();

    void on_actFile_Reload_triggered();


    void on_btnFormatConvert_clicked();


    void on_btnGetRGB565_clicked();

    void on_btnSaveDataFile_clicked();

    void on_actFile_Save_triggered();

    void on_actFile_SaveAs_triggered();

    void on_actFile_Print_triggered();

    void on_actFile_Preview_triggered();

    void on_actImg_ZoomIn_triggered();

    void on_actImg_ZoomOut_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
