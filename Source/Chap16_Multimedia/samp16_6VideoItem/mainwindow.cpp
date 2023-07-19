#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);       //创建播放器
    QAudioOutput *audioOutput= new QAudioOutput(this);
    player->setAudioOutput(audioOutput);   //设置音频输出通道

    QGraphicsScene *scene = new QGraphicsScene(this);   //创建场景
    ui->graphicsView->setScene(scene);     //为视图设置场景
    videoItem = new QGraphicsVideoItem;    //常见视频显示图形项
    videoItem->setSize(QSizeF(360, 240));
    videoItem->setFlags(QGraphicsItem::ItemIsMovable
                        | QGraphicsItem::ItemIsSelectable);
//                        | QGraphicsItem::ItemIsFocusable);
    scene->addItem(videoItem);
    player->setVideoOutput(videoItem);      //设置视频显示图形项

    connect(player,&QMediaPlayer::playbackStateChanged,this, &MainWindow::do_stateChanged);
    connect(player,&QMediaPlayer::positionChanged,this, &MainWindow::do_positionChanged);
    connect(player,&QMediaPlayer::durationChanged,this, &MainWindow::do_durationChanged);


    QGraphicsSimpleTextItem   *item2=new QGraphicsSimpleTextItem("海风吹，海浪涌");
    QFont font=item2->font();
    font.setPointSize(20);
    item2->setFont(font);
    item2->setPos(0,0);
    item2->setBrush(QBrush(Qt::blue));
    item2->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
//                    | QGraphicsItem::ItemIsFocusable);
    scene->addItem(item2);

//    //一个圆，中心位于scene的边缘
//    QGraphicsEllipseItem   *item3=new QGraphicsEllipseItem(50,50,50,50); //矩形框内创建椭圆,绘图项的局部坐标，左上角(-100,-50)，宽200，高100
//    item3->setPos(100,100);
//    item3->setBrush(QBrush(Qt::red));
//    item3->setFlags(QGraphicsItem::ItemIsMovable
//                    | QGraphicsItem::ItemIsSelectable
//                    | QGraphicsItem::ItemIsFocusable);
//    scene->addItem(item3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{
    bool isPlaying = (state==QMediaPlayer::PlayingState);
    ui->btnPlay->setEnabled(!isPlaying);
    ui->btnPause->setEnabled(isPlaying);
    ui->btnStop->setEnabled(isPlaying);
}


void MainWindow::do_durationChanged(qint64 duration)
{
    ui->sliderPosition->setMaximum(duration);

    int   secs=duration/1000;//秒
    int   mins=secs/60; //分钟
    secs=secs % 60;//余数秒
    durationTime=QString::asprintf("%d:%d",mins,secs);
    ui->LabRatio->setText(positionTime+"/"+durationTime);
}

void MainWindow::do_positionChanged(qint64 position)
{
    if (ui->sliderPosition->isSliderDown())
        return;

    ui->sliderPosition->setSliderPosition(position);

    int   secs=position/1000;   //秒
    int   mins=secs/60;         //分钟
    secs=secs % 60;             //余数秒
    positionTime=QString::asprintf("%d:%d",mins,secs);
    ui->LabRatio->setText(positionTime+"/"+durationTime);
}

void MainWindow::on_btnAdd_clicked()
{
    QString curPath=QDir::homePath();
    QString dlgTitle="选择视频文件";
    QString filter="视频文件(*.wmv, *.mp4);;所有文件(*.*)";
    QString aFile=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if (aFile.isEmpty())
        return;

    QFileInfo   fileInfo(aFile);
    ui->LabCurMedia->setText(fileInfo.fileName());

    player->setSource(QUrl::fromLocalFile(aFile));
    player->play();
}

void MainWindow::on_btnPlay_clicked()
{
    player->play();
}

void MainWindow::on_btnPause_clicked()
{
    player->pause();
}

void MainWindow::on_btnStop_clicked()
{
    player->stop();
}

void MainWindow::on_sliderVolumn_valueChanged(int value)
{
    player->audioOutput()->setVolume(value);
}

void MainWindow::on_btnSound_clicked()
{
    bool mute=player->audioOutput()->isMuted();
    player->audioOutput()->setMuted(!mute);
    if (mute)
        ui->btnSound->setIcon(QIcon(":/images/images/volumn.bmp"));
    else
        ui->btnSound->setIcon(QIcon(":/images/images/mute.bmp"));
}

void MainWindow::on_sliderPosition_valueChanged(int value)
{
    player->setPosition(value);
}


void MainWindow::on_btnZoomIn_clicked()
{//放大
    qreal factor=videoItem->scale();
    videoItem->setScale(factor+0.1);
}


void MainWindow::on_btnZoomOut_clicked()
{//缩小
    qreal factor=videoItem->scale();
    if (factor>=0.2)
        videoItem->setScale(factor-0.1);
}

