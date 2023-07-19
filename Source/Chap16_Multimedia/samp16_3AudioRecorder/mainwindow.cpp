#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>
#include    <QFileInfo>

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (recorder->recorderState() != QMediaRecorder::StoppedState)
    {
        QMessageBox::information(this,"提示","正在录音，不能退出");
        event->ignore();
    }
    else
        event->accept();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    session = new QMediaCaptureSession(this);  //QMediaCaptureSession是采集音频、视频的类

    QAudioInput *audioInput= new QAudioInput(this);
    session->setAudioInput(audioInput);     // session 需要设置音频输入

    recorder= new QMediaRecorder(this);
    session->setRecorder(recorder);         //为session 设置 recorder

    connect(recorder,&QMediaRecorder::recorderStateChanged,
            this,&MainWindow::do_stateChanged);

    connect(recorder, &QMediaRecorder::durationChanged,
            this, &MainWindow::do_durationChanged);

    if (QMediaDevices::defaultAudioInput().isNull())    //如果没有默认的音频输入设备
    {
        ui->groupBoxDevice->setTitle("录音设置（无设备）");
        ui->actRecord->setEnabled(false);
        QMessageBox::information(this,"提示", "无音频输入设备");
        return;  //无音频录入设备
    }

     //音频录入设备列表
    foreach (QAudioDevice device, QMediaDevices::audioInputs())
        ui->comboDevices->addItem(device.description(), QVariant::fromValue(device));

    QMediaFormat format;        //默认的格式对象
    //支持的编码格式
    foreach (QMediaFormat::AudioCodec encoder, format.supportedAudioCodecs(QMediaFormat::Encode))
        ui->comboCodec->addItem(QMediaFormat::audioCodecDescription(encoder),
                                QVariant::fromValue(encoder));
    //支持的文件格式
    foreach (QMediaFormat::FileFormat fileFormat, format.supportedFileFormats(QMediaFormat::Encode))
        ui->comboFileFormat->addItem(QMediaFormat::fileFormatDescription(fileFormat),
                                     QVariant::fromValue(fileFormat));
    //采样频率
    int minSampRate=audioInput->device().minimumSampleRate();   //输入设备支持的最低采样率
    ui->comboSampleRate->addItem(QString("Minimum %1").arg(minSampRate), minSampRate);
    int maxSampRate=audioInput->device().maximumSampleRate();   //输入设备支持的最高采样率
    ui->comboSampleRate->addItem(QString("Maximum %1").arg(maxSampRate), maxSampRate);
    ui->comboSampleRate->addItem("16000", 16000);   //添加了用户数据
    ui->comboSampleRate->addItem("44100", 44100);
    ui->comboSampleRate->addItem("48000", 48000);
    ui->comboSampleRate->addItem("88200", 88200);

    //通道数
    int minChan=audioInput->device().minimumChannelCount();  //最少通道数
    ui->comboChannels->addItem(QString("Minimum %1").arg(minChan), minChan);
    int maxChan=audioInput->device().maximumChannelCount();  //最多通道数
    ui->comboChannels->addItem(QString("Maximum %1").arg(maxChan), maxChan);
    ui->comboChannels->addItem("1", 1);
    ui->comboChannels->addItem("2", 2);

    //固定品质
    ui->sliderQuality->setRange(0, int(QImageCapture::VeryHighQuality));
    ui->sliderQuality->setValue(int(QImageCapture::NormalQuality));

    //固定比特率
    ui->comboBitrate->addItem("32000");
    ui->comboBitrate->addItem("64000");
    ui->comboBitrate->addItem("96000");
    ui->comboBitrate->addItem("128000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_stateChanged(QMediaRecorder::RecorderState state)
{ //录音状态变化
    bool isRecording = state==QMediaRecorder::RecordingState;   //正在录制
    ui->actRecord->setEnabled(!isRecording);
    ui->actPause->setEnabled(isRecording);
    ui->actStop->setEnabled(isRecording);

    ui->btnGetFile->setEnabled(state==QMediaRecorder::StoppedState);
}

void MainWindow::do_durationChanged(qint64 duration)
{//录音持续时间变化
    ui->labPassTime->setText(QString("已录制 %1 秒").arg(duration / 1000));
}

//void MainWindow::processBuffer(const QAudioBuffer &buffer)
//{ //处理探测到的缓冲区
//    ui->spin_byteCount->setValue(buffer.byteCount());//缓冲区字节数
//    ui->spin_duration->setValue(buffer.duration()/1000);//缓冲区时长
//    ui->spin_frameCount->setValue(buffer.frameCount());//缓冲区帧数
//    ui->spin_sampleCount->setValue(buffer.sampleCount());//缓冲区采样数

//    QAudioFormat audioFormat=buffer.format();//缓冲区格式
//    ui->spin_channelCount->setValue(audioFormat.channelCount()); //通道数
//    ui->spin_sampleSize->setValue(audioFormat.sampleSize());//采样大小
//    ui->spin_sampleRate->setValue(audioFormat.sampleRate());//采样率
//    ui->spin_bytesPerFrame->setValue(audioFormat.bytesPerFrame());//每帧字节数

//    if (audioFormat.byteOrder()==QAudioFormat::LittleEndian)
//        ui->edit_byteOrder->setText("LittleEndian");//字节序
//    else
//        ui->edit_byteOrder->setText("BigEndian");

//    ui->edit_codec->setText(audioFormat.codec());//编码格式

//    if (audioFormat.sampleType()==QAudioFormat::SignedInt)//采样点类型
//        ui->edit_sampleType->setText("SignedInt");
//    else if(audioFormat.sampleType()==QAudioFormat::UnSignedInt)
//        ui->edit_sampleType->setText("UnSignedInt");
//    else if(audioFormat.sampleType()==QAudioFormat::Float)
//        ui->edit_sampleType->setText("Float");
//    else
//        ui->edit_sampleType->setText("Unknown");

//}

void MainWindow::on_actRecord_triggered()
{ //开始录音
    if (recorder->recorderState() == QMediaRecorder::PausedState)
    {//若是暂停状态，就继续录音
        recorder->record();
        return;
    }

    QString selectedFile=ui->editOutputFile->text().trimmed();
    if (selectedFile.isEmpty())
    {
        QMessageBox::critical(this,"错误","请先设置录音输出文件");
        return;
    }
    if (QFile::exists(selectedFile))
        QFile::remove(selectedFile);
    recorder->setOutputLocation(QUrl::fromLocalFile(selectedFile)); //设置输出文件

//设置session的输入设备
//    QVariant var =ui->comboDevices->itemData(ui->comboDevices->currentIndex());
//    QAudioDevice audioDevice= var.value<QAudioDevice>();
//    session->audioInput()->setDevice(audioDevice);
    session->audioInput()->setDevice(QMediaDevices::defaultAudioInput());

//设置 recorder 的 mediaFormat 参数，包括文件格式和编码格式
    QMediaFormat mediaFormat;
    QVariant var=ui->comboCodec->itemData(ui->comboCodec->currentIndex());
    QMediaFormat::FileFormat fileFormat= var.value<QMediaFormat::FileFormat>();
    mediaFormat.setFileFormat(fileFormat);  //设置文件格式

    var=ui->comboFileFormat->itemData(ui->comboFileFormat->currentIndex());
    QMediaFormat::AudioCodec audioCodec =var.value<QMediaFormat::AudioCodec>();
    mediaFormat.setAudioCodec(audioCodec);  //设置编码格式

    recorder->setMediaFormat(mediaFormat);  //设置mediaFormat

//设置 recorder 的其他参数
    var=ui->comboSampleRate->itemData(ui->comboSampleRate->currentIndex());
    recorder->setAudioSampleRate(var.toInt());      //设置采样率
    var=ui->comboChannels->itemData(ui->comboChannels->currentIndex());
    recorder->setAudioChannelCount(var.toInt());    //设置通道数
    recorder->setAudioBitRate(ui->comboBitrate->currentText().toInt());         //设置比特率
    recorder->setQuality(QMediaRecorder::Quality(ui->sliderQuality->value()));  //设置品质
    if (ui->radioQuality->isChecked())              //设置编码模式
        recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);     //固定品质
    else
        recorder->setEncodingMode(QMediaRecorder::ConstantBitRateEncoding);     //固定比特率

    recorder->record();
}

void MainWindow::on_btnGetFile_clicked()
{ //设置保存文件
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="选择输出文件"; //对话框标题
    QString filter="所有文件(*.*);;MP3文件(*.mp3);;WMA文件(*.wma);;MP4文件(*.mp4)"; //文件过滤器
    QString selectedFile=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

    if (!selectedFile.isEmpty())
    {
        ui->editOutputFile->setText(selectedFile);
        QFileInfo  fileInfo(selectedFile);
        QDir::setCurrent(fileInfo.absolutePath());
    }
}

void MainWindow::on_actPause_triggered()
{ //暂停
    recorder->pause();
}

void MainWindow::on_actStop_triggered()
{//停止
    recorder->stop();
}
