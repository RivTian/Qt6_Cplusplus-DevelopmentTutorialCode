#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QtMultimedia>

#include    <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QMediaCaptureSession  *session;      //抓取管理器
    QCamera     *camera;                //摄像头

    QImageCapture   *imageCapture;      //抓图器
    QSoundEffect    *soundEffect;       //用于拍照时播放快门音效

    QMediaRecorder  *recorder;          //录像

    QLabel   *labDuration;  //用于状态栏显示信息
    QLabel   *labInfo;
    QLabel   *labFormatInfo;
    bool    m_isWorking=false;  //是否已开启摄像头

//    void    iniCamera();//初始化
//    void    iniImageCapture();//初始化静态画图
//    void    iniVideoRecorder();//初始化视频录制

    void    showCameraDeviceInfo(QCameraDevice *device);
    void    showCameraSupportFeatures(QCamera *aCamera);
    void    closeEvent(QCloseEvent *event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void do_camera_changed(int index);  //与摄像头下拉列表框的信号关联

//与QCamera的信号关联
    void do_camera_activeChanged(bool active);

//与QImageCapture的信号关联
    void do_image_readyForCapture(bool ready);
    void do_image_captured(int id, const QImage &preview);
    void do_image_saved(int id, const QString &fileName);

//与QMediaRecorder的信号关联
    void do_recorder_duration(qint64 duration);
    void do_recorder_stateChanged(QMediaRecorder::RecorderState state);
    void do_recorder_error(QMediaRecorder::Error error, const QString &errorString);


    void on_actStartCamera_triggered();

    void on_actStopCamera_triggered();

    void on_actCapture_triggered();

    void on_actVideoRecord_triggered();

    void on_actVideoStop_triggered();

    void on_btnVideoFile_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
