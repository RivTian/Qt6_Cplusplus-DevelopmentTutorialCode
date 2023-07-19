#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QtMultimedia>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QMediaCaptureSession *session;
    QMediaRecorder  *recorder;

    void closeEvent(QCloseEvent *event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
//自定义槽函数
    void do_stateChanged(QMediaRecorder::RecorderState state);

    void do_durationChanged(qint64 duration);

    void on_actRecord_triggered();

    void on_btnGetFile_clicked();

    void on_actPause_triggered();

    void on_actStop_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
