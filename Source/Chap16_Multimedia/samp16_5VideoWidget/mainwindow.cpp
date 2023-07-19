#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);        //创建视频播放器
    QAudioOutput *audioOutput= new QAudioOutput(this);
    player->setAudioOutput(audioOutput);        //设置音频输出通道
    player->setVideoOutput(ui->videoWidget);    //设置视频显示组件

    ui->videoWidget->setMediaPlayer(player);//设置显示组件的关联播放器

    connect(player,&QMediaPlayer::playbackStateChanged,this, &MainWindow::do_stateChanged);

    connect(player,&QMediaPlayer::positionChanged,this, &MainWindow::do_positionChanged);

    connect(player,&QMediaPlayer::durationChanged,this, &MainWindow::do_durationChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{//播放器状态变化
    bool isPlaying = (state==QMediaPlayer::PlayingState);
    ui->btnPlay->setEnabled(!isPlaying);
    ui->btnPause->setEnabled(isPlaying);
    ui->btnStop->setEnabled(isPlaying);
}

void MainWindow::do_durationChanged(qint64 duration)
{//文件时长变化
    ui->sliderPosition->setMaximum(duration);

    int   secs=duration/1000;   //秒
    int   mins=secs/60;         //分钟
    secs=secs % 60;             //余数秒
    durationTime=QString::asprintf("%d:%d",mins,secs);
    ui->LabRatio->setText(positionTime+"/"+durationTime);
}

void MainWindow::do_positionChanged(qint64 position)
{//文件播放位置变化
    if (ui->sliderPosition->isSliderDown())
        return;     //如果正在拖动滑条，退出
    ui->sliderPosition->setSliderPosition(position);
    int   secs=position/1000;   //秒
    int   mins=secs/60;         //分钟
    secs=secs % 60;             //余数秒
    positionTime=QString::asprintf("%d:%d",mins,secs);
    ui->LabRatio->setText(positionTime+"/"+durationTime);
}

void MainWindow::on_btnAdd_clicked()
{//打开文件
    QString curPath=QDir::homePath();
    QString dlgTitle="选择视频文件";
    QString filter="视频文件(*.wmv, *.mp4);;所有文件(*.*)";
    QString aFile=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if (aFile.isEmpty())
      return;

    QFileInfo   fileInfo(aFile);
    ui->labCurMedia->setText(fileInfo.fileName());

    player->setSource(QUrl::fromLocalFile(aFile));  //设置播放文件
    player->play();
}

void MainWindow::on_btnPlay_clicked()
{//播放
    player->play();
}

void MainWindow::on_btnPause_clicked()
{//暂停
    player->pause();
}

void MainWindow::on_btnStop_clicked()
{//停止
    player->stop();
}

void MainWindow::on_sliderVolumn_valueChanged(int value)
{//调节音量
    player->audioOutput()->setVolume(value/100.0);
}

void MainWindow::on_btnSound_clicked()
{//静音按钮
    bool mute=player->audioOutput()->isMuted();
    player->audioOutput()->setMuted(!mute);
    if (mute)
        ui->btnSound->setIcon(QIcon(":/images/images/volumn.bmp"));
    else
        ui->btnSound->setIcon(QIcon(":/images/images/mute.bmp"));
}

void MainWindow::on_sliderPosition_valueChanged(int value)
{//播放位置
   player->setPosition(value);
}

void MainWindow::on_pushButton_clicked()
{//全屏按钮
    ui->videoWidget->setFullScreen(true);
}
