#include    "mainwindow.h"
#include    "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>
#include    <QSoundEffect>

//void MainWindow::iniCamera()
//{//  创建  QCamera对象
//}

//void MainWindow::iniImageCapture()
//{//创建 QCameraImageCapture对象
//}

//void MainWindow::iniVideoRecorder()
//{//创建QMediaRecorder对象
//}

void MainWindow::showCameraDeviceInfo(QCameraDevice *device)
{//显示摄像头设备信息
    ui->comboCam_Position->setCurrentIndex(device->position()); //摄像头位置

    ui->comboCam_PhotoRes->clear();     //支持的拍照分辨率
    ui->comboImage_Resolution->clear(); //拍照使用的分辨率
    foreach(QSize size, device->photoResolutions())
    {
        QString str=QString::asprintf("%d X %d",size.width(),size.height());
        ui->comboCam_PhotoRes->addItem(str);            //支持的拍照分辨率
        ui->comboImage_Resolution->addItem(str,size);   //拍照使用的分辨率，添加QSize用户数据
    }

    ui->comboCam_VideoRes->clear();     //支持的视频分辨率
    ui->comboCam_FrameRate->clear();    //支持的帧率范围
    ui->comboVideo_Resolution->clear(); //录像用的分辨率
    foreach(QCameraFormat format, device->videoFormats())
    {
        QSize size=format.resolution();
        QString str=QString::asprintf("%d X %d",size.width(),size.height());
        ui->comboCam_VideoRes->addItem(str);
        ui->comboVideo_Resolution->addItem(str,size);   //添加QSize用户数据

        str=QString::asprintf("%.1f ~ %.1f",format.minFrameRate(),format.maxFrameRate());
        ui->comboCam_FrameRate->addItem(str);
    }
}

void MainWindow::showCameraSupportFeatures(QCamera *aCamera)
{//显示摄像头支持的特性
    QCamera::Features features= aCamera->supportedFeatures();

    bool    supported=features.testFlag(QCamera::Feature::ColorTemperature);
    ui->chkBoxCam_Color->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::ExposureCompensation);
    ui->chkBoxCam_Exposure->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::IsoSensitivity);
    ui->chkBoxCam_Iso->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::CustomFocusPoint);
    ui->chkBoxCam_Custom->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::ManualExposureTime);
    ui->chkBoxCam_Manual->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::FocusDistance);
    ui->chkBoxCam_Focus->setChecked(supported);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_isWorking)
    {
        if(recorder->recorderState() == QMediaRecorder::RecordingState) //正在录制视频
            recorder->stop();   //停止录制视频
        camera->stop();         //关闭摄像头
    }
    event->accept();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labDuration=new QLabel("录制时间: ");
    labDuration->setMinimumWidth(120);
    ui->statusBar->addWidget(labDuration);

    labFormatInfo=new QLabel("图片分辨率: ");
    labFormatInfo->setMinimumWidth(150);
    ui->statusBar->addWidget(labFormatInfo);

    labInfo=new QLabel("信息");
    ui->statusBar->addPermanentWidget(labInfo);

//1. 发现摄像头
    QCameraDevice  defaultCameraDevice= QMediaDevices::defaultVideoInput();
    if (defaultCameraDevice.isNull())
    {
        QMessageBox::critical(this,"提示","没有发现摄像头");
        return;
    }
    QByteArray defaultCameraID=defaultCameraDevice.id();
    ui->actStartCamera->setEnabled(true);

//    disconnect(ui->comboCam_List);  //解除与comboCam_List连接的所有槽函数
    int index=0;
    for(int i=0; i<QMediaDevices::videoInputs().size(); i++)
    {
        QCameraDevice device=QMediaDevices::videoInputs().at(i);
        if (device.id() == defaultCameraID)
        {
            ui->comboCam_List->addItem(device.description()+"(默认)",QVariant::fromValue(device));
            index=i;
        }
        else
            ui->comboCam_List->addItem(device.description(),QVariant::fromValue(device));
    }

    if (ui->comboCam_List->currentIndex() != index)
        ui->comboCam_List->setCurrentIndex(index);

//    foreach(QCameraDevice device, QMediaDevices::videoInputs())
//    {
//        if (device.isDefault())         //是默认设备
//            ui->comboCam_List->addItem(device.description()+"(默认)",QVariant::fromValue(device));
//        else
//            ui->comboCam_List->addItem(device.description(),QVariant::fromValue(device));
//    }

//    ui->comboCam_List->addItem(cameraDevice.description());
//    showCameraDeviceInfo(&cameraDevice);

//    ui->comboCam_List->addItem(cameraDevice.description());

//2. 创建抓取管理器session和各个设备
    session = new QMediaCaptureSession(this);       //抓取管理器
    session->setVideoOutput(ui->videoPreview);      //设置视频输出组件，用于视频预览
//2.1 创建QAudioInput对象，用用音频输入
    QAudioInput  *audioInput=new QAudioInput(this);
    audioInput->setDevice(QMediaDevices::defaultAudioInput());
    session->setAudioInput(audioInput);     //设置音频输入设备

//2.2 创建QCamera对象,用于控制摄像头
    camera = new QCamera(this);        //摄像头
    camera->setCameraDevice(defaultCameraDevice);   //
    session->setCamera(camera);        //为session设置摄像头
    connect(camera, &QCamera::activeChanged,this, &MainWindow::do_camera_activeChanged);
    connect(ui->comboCam_List,&QComboBox::currentIndexChanged,
            this, &MainWindow::do_camera_changed);
    do_camera_changed(ui->comboCam_List->currentIndex());

//2.3 创建QImageCapture对象,用于拍照
    imageCapture = new QImageCapture(this);     //抓图器
    imageCapture->setQuality(QImageCapture::VeryHighQuality);
    session->setImageCapture(imageCapture);     //为session设置抓图器
    connect(imageCapture,&QImageCapture::readyForCaptureChanged,
            this, &MainWindow::do_image_readyForCapture);
    connect(imageCapture,&QImageCapture::imageCaptured,
            this,&MainWindow::do_image_captured);
    connect(imageCapture, &QImageCapture::imageSaved,
            this, &MainWindow::do_image_saved);

    soundEffect =new QSoundEffect(this);     //用于播放音效，拍照时的快门声音
    QString filename=":/sound/images/shutter.wav";
    soundEffect->setSource(QUrl::fromLocalFile(filename));

//2.4 创建QMediaRecorder对象，用于录制视频
    recorder = new QMediaRecorder(this);  //创建recorder
    session->setRecorder(recorder);       //为session设置recorder
    connect(recorder, &QMediaRecorder::durationChanged,
            this, &MainWindow::do_recorder_duration);
    connect(recorder, &QMediaRecorder::recorderStateChanged,
            this, &MainWindow::do_recorder_stateChanged);
    connect(recorder, &QMediaRecorder::errorOccurred,
            this, &MainWindow::do_recorder_error);

//3. 视频编码和文件格式，添加到录像设置的下拉列表框里，用于选择
    QMediaFormat format;
    foreach(QMediaFormat::VideoCodec codec, format.supportedVideoCodecs(QMediaFormat::Encode))
        ui->comboVideo_Codec->addItem(QMediaFormat::videoCodecDescription(codec),
                                      QVariant::fromValue(codec));
    foreach(QMediaFormat::FileFormat fileFormat, format.supportedFileFormats(QMediaFormat::Encode))
        ui->comboVideo_FileFormat->addItem(QMediaFormat::fileFormatDescription(fileFormat),
                                      QVariant::fromValue(fileFormat));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_camera_activeChanged(bool active)
{//摄像头状态变化
    ui->actStartCamera->setEnabled(!active);
    ui->actStopCamera->setEnabled(active);

    ui->actVideoRecord->setEnabled(active);
    ui->comboCam_List->setEnabled(!active);

    m_isWorking = active;
}

void MainWindow::do_image_readyForCapture(bool ready)
{//与 readyForCaptureChanged()信号关联
    ui->actCapture->setEnabled(ready);
}

void MainWindow::do_image_captured(int id, const QImage &preview)
{//与 imageCaptured()信号关联
    Q_UNUSED(id);

//    QSize size=imageCapture->resolution();   //被设置了，但是不起作用， Qt  6.2.2的 bug
//    QString str=QString::asprintf("实际图片分辨率= %d X %d",size.width(),size.height());
    QString str=QString::asprintf("实际图片分辨率= %d X %d",preview.width(),preview.height());
    labFormatInfo->setText(str);

    QImage scaledImage = preview.scaledToWidth(ui->scrollArea->width()-30);
    ui->labImage->setPixmap(QPixmap::fromImage(scaledImage));

    if(! ui->chkBox_SaveToFile->isChecked())
        labInfo->setText("图片未保存为文件");
}

void MainWindow::do_image_saved(int id, const QString &fileName)
{//与 imageSaved()信号关联
    Q_UNUSED(id);
    labInfo->setText("图片保存为文件： "+fileName);
}

void MainWindow::do_recorder_duration(qint64 duration)
{//与  durationChanged()信号关联
    labDuration->setText(QString::asprintf("录制时间:%.1f 秒 ",duration/1000.0));
}

void MainWindow::do_recorder_stateChanged(QMediaRecorder::RecorderState state)
{//与 recorderStateChanged()信号关联
    ui->actVideoRecord->setEnabled(state == QMediaRecorder::StoppedState);
    ui->actVideoStop->setEnabled(state == QMediaRecorder::RecordingState);
}

void MainWindow::do_recorder_error(QMediaRecorder::Error error, const QString &errorString)
{//与 errorOccurred()信号关联
    Q_UNUSED(error);
    labInfo->setText(errorString);
}

void MainWindow::on_actStartCamera_triggered()
{//开启摄像头
   camera->start();
}

void MainWindow::on_actStopCamera_triggered()
{//关闭摄像头
    if (recorder->recorderState() == QMediaRecorder::RecordingState)
        recorder->stop(); //停止录像
    camera->stop();
}

void MainWindow::on_actCapture_triggered()
{//"拍照" 按钮
    int index=ui->comboImage_Quality->currentIndex();
    imageCapture->setQuality(QImageCapture::Quality(index));  //图片文件编码品质

    index=ui->comboImage_Resolution->currentIndex();
    QVariant var=ui->comboImage_Resolution->itemData(index);    //用户数据
    imageCapture->setResolution(var.toSize());      //设置分辨率

    //    imageCapture->setResolution(640, 480);  //直接设置也不起作用，是Qt 6.2.2中的bug
    if (ui->chkBox_SaveToFile->isChecked())
        imageCapture->captureToFile();  //拍照并保存到文件
    else
        imageCapture->capture();        //拍照
    if (ui->chkBox_Sound->isChecked())
        soundEffect->play();    //播放快门音效
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_actVideoRecord_triggered()
{//"开始录像"按钮
    QString selectedFile=ui->editVideo_OutputFile->text().trimmed();
    if (selectedFile.isEmpty())
    {
        QMessageBox::critical(this,"错误","请先设置录像输出文件");
        return;
    }

    if (QFile::exists(selectedFile))
        if (!QFile::remove(selectedFile))
        {
            QMessageBox::critical(this,"错误","所设置录像输出文件被占用，无法删除");
            return;
        }

    recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);   //按固定品质编码
    int index =ui->comboViedo_Quality->currentIndex();
    recorder->setQuality(QMediaRecorder::Quality(index));  //编码品质

    //设置媒介格式
    QMediaFormat mediaFormat;
    index=ui->comboVideo_Codec->currentIndex();
    QVariant var= ui->comboVideo_Codec->itemData(index);    //获取用户数据
    QMediaFormat::VideoCodec codec=var.value<QMediaFormat::VideoCodec>();
    mediaFormat.setVideoCodec(codec);       //设置视频编码

    index=ui->comboVideo_FileFormat->currentIndex();
    var=ui->comboVideo_FileFormat->itemData(index);
    QMediaFormat::FileFormat fileFormat=var.value<QMediaFormat::FileFormat>();
    mediaFormat.setFileFormat(fileFormat);  //设置文件格式

    recorder->setMediaFormat(mediaFormat);  //设置媒介格式

    index=ui->comboVideo_Resolution->currentIndex();
    var=ui->comboVideo_Resolution->itemData(index); //获取用户数据
    recorder->setVideoResolution(var.toSize());     //设置视频分辨率

    labInfo->clear();
    recorder->setOutputLocation(QUrl::fromLocalFile(selectedFile)); //设置输出文件
    recorder->record();
}

void MainWindow::on_actVideoStop_triggered()
{//停止录像
    recorder->stop();
}

void MainWindow::on_btnVideoFile_clicked()
{
    QString curPath=QDir::homePath();
    QString dlgTitle="选择保存文件";
    QString filter="MP4视频文件(*.mp4);;WMV视频文件(*.wmv);;所有文件(*.*)";
    QString selectedFile=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (!selectedFile.isEmpty())
        ui->editVideo_OutputFile->setText(selectedFile);
}

void MainWindow::do_camera_changed(int index)
{
    QVariant var=ui->comboCam_List->itemData(index);
    QCameraDevice device = var.value<QCameraDevice>();
    showCameraDeviceInfo(&device);      //显示摄像头设备信息

    camera->setCameraDevice(device);    //重新设置摄像头设备
    showCameraSupportFeatures(camera);  //显示摄像头支持的特性
}

