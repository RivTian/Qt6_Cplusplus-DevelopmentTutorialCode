#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QtMultimedia>

#include    <QGraphicsVideoItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QMediaPlayer  *player;
    QGraphicsVideoItem *videoItem;   //视频显示图形项

    QString  durationTime;
    QString  positionTime;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //自定义槽函数
    void do_stateChanged(QMediaPlayer::PlaybackState state);
    void do_durationChanged(qint64 duration);
    void do_positionChanged(qint64 position);

    void on_btnAdd_clicked();

    void on_btnPlay_clicked();

    void on_btnPause_clicked();

    void on_btnStop_clicked();


    void on_sliderVolumn_valueChanged(int value);

    void on_btnSound_clicked();


    void on_sliderPosition_valueChanged(int value);

    void on_btnZoomIn_clicked();

    void on_btnZoomOut_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
