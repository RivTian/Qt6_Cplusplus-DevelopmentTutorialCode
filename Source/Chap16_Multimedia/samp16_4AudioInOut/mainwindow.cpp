#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QMessageBox>


void MainWindow::iniChart()
{//创建图表
    QChart *chart = new QChart;
    chart->setTitle("音频输入原始信号");
    ui->chartView->setChart(chart);
    lineSeries= new QLineSeries();   //创建序列
    chart->addSeries(lineSeries);
    lineSeries->setUseOpenGL(true);  //使用OpenGL加速

    QValueAxis *axisX = new QValueAxis;  //X坐标轴
    axisX->setRange(0, m_bufferSize);    //X数据范围
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");

    QValueAxis *axisY = new QValueAxis;  //Y坐标轴
    axisY->setRange(0, 256);             //UInt8采样，数据范围0~255
    axisY->setTitleText("Audio Level");

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);

    chart->legend()->hide();        //隐藏图例
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniChart();     //创建图表

    QAudioDevice device=QMediaDevices::defaultAudioInput();  //默认音频输入设备
    if (device.isNull())
    {
        ui->actStart->setEnabled(false);
        ui->groupBoxDevice->setTitle("音频输入设置(无设备)");
        QMessageBox::information(this,"提示","无音频输入设备");
        return;
    }
    ui->comboDevices->addItem(device.description());        //只添加默认音频输入设备

//首选音频格式
    QAudioFormat audioFormat =device.preferredFormat();     //音频输入设备的首选音频格式
    ui->comboSampFormat->setCurrentIndex(audioFormat.sampleFormat());   //采样点格式

    ui->spinSampRate->setValue(audioFormat.sampleRate());   //采样频率
    int minRate=device.minimumSampleRate();
    int maxRate=device.maximumSampleRate();
    ui->labSampRateRange->setText(QString::asprintf("范围: %d~%d",minRate,maxRate));
    ui->spinSampRate->setRange(minRate, maxRate);

    ui->spinChanCount->setValue(audioFormat.channelCount()); //通道个数
    int minChan=device.minimumChannelCount();
    int maxChan=device.maximumChannelCount();
    ui->labChanCountRange->setText(QString::asprintf("范围:%d~%d",minChan,maxChan));
    ui->spinChanCount->setRange(minChan, maxChan);

    ui->spinBytesPerSamp->setValue(audioFormat.bytesPerSample());  //每个采样点的字节数
    ui->spinBytesPerFrame->setValue(audioFormat.bytesPerFrame());  //每帧字节数
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::do_IODevice_update(qint64 blockSize)
{
    float time=audioSource->processedUSecs()/1000; //ms
    QString str=QString::asprintf("已录制时间 =%.1f 秒", time/1000);
    ui->labBufferSize->setText(str);
    ui->labBlockSize->setText(QString("实际数据块字节数=%1").arg(blockSize));
}

void MainWindow::do_sink_stateChanged(QAudio::State state)
{
    if (state == QAudio::IdleState)  //播放结束后的空闲状态
    {
        sinkFileDevice.close();     //关闭文件
        audioSink->stop();          //停止播放
        audioSink->deleteLater();   //在主循环里删除对象
        ui->actPlayFile->setEnabled(true);
        m_isWorking=false;          //表示没有设备在工作了
    }
}

void MainWindow::on_actStart_triggered()
{//“开始采集”按钮
    if(ui->comboSampFormat->currentIndex()==0)
    {
        QMessageBox::critical(this,"错误","请设置采样点格式");
        return;
    }

    bool saveToFile=ui->chkBoxSaveToFile->isChecked();  //是否保存到文件
    QString fileName= ui->editFileName->text().trimmed();
    if ((saveToFile) && (fileName.isEmpty()))
    {
        QMessageBox::critical(this,"错误","请设置要保存的文件");
        return;
    }

    //设置音频格式
    QAudioFormat daqFormat;
    daqFormat.setSampleRate(ui->spinSampRate->value());      //采样频率
    daqFormat.setChannelCount(ui->spinChanCount->value());   //通道个数
    int index=ui->comboSampFormat->currentIndex();
    daqFormat.setSampleFormat(QAudioFormat::SampleFormat(index));  //采样点格式

    audioSource = new QAudioSource(daqFormat, this);
    audioSource->setBufferSize(m_bufferSize);    //设置缓冲区大小,如10000
    audioSource->setVolume(1);                   //设置录音音量， 0~1

    myDevice = new TMyDevice(this);
    connect(myDevice,&TMyDevice::updateBlockSize,this, &MainWindow::do_IODevice_update);

    bool showWave=ui->chkBoxShowWave->isChecked();      //是否实时显示曲线
    myDevice->openDAQ(m_bufferSize,showWave,lineSeries,saveToFile, fileName);
    audioSource->start(myDevice); //以流设备作为参数，开始录入音频输入数据

    m_isWorking=true;       //表示有设置在工作，不允许关闭窗口
    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);
    ui->actPlayFile->setEnabled(false);     //"播放文件"按钮
}

void MainWindow::on_actStop_triggered()
{//"停止采集"按钮
    audioSource->stop();   //停止采集
    myDevice->closeDAQ();  //IO设备停止记录
    delete myDevice;       //删除对象
    delete audioSource;    //删除对象

    m_isWorking=false;      //表示没有设备在工作了
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    ui->actPlayFile->setEnabled(ui->chkBoxSaveToFile->isChecked()); //"播放文件"按钮
}


void MainWindow::on_btnGetFile_clicked()
{//"数据文件" 按钮
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="选择输出文件"; //对话框标题
    QString filter="原始音频数据文件(*.raw);;所有文件(*.*)"; //文件过滤器
    QString selectedFile=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (!selectedFile.isEmpty())
    {
        ui->editFileName->setText(selectedFile);
        QFileInfo  fileInfo(selectedFile);
        QDir::setCurrent(fileInfo.absolutePath());
    }
}

void MainWindow::on_chkBoxSaveToFile_clicked(bool checked)
{//"数据记录到文件" CheckBox
    ui->btnGetFile->setEnabled(checked);
    ui->editFileName->setEnabled(checked);
}

void MainWindow::on_actTest_triggered()
{//"测试音频格式"按钮
    QAudioFormat daqFormat;
    daqFormat.setSampleRate(ui->spinSampRate->value());     //采样频率
    daqFormat.setChannelCount(ui->spinChanCount->value());  //通道数
    int index=ui->comboSampFormat->currentIndex();
    daqFormat.setSampleFormat(QAudioFormat::SampleFormat(index));   //采样点格式

    QAudioDevice device=QMediaDevices::defaultAudioInput();  //默认音频输入设备
    if (device.isFormatSupported(daqFormat))
        QMessageBox::information(this,"提示","默认设备支持所选格式 ");
    else
        QMessageBox::critical(this,"提示","默认设备不支持所选格式 ");
}


void MainWindow::on_comboSampFormat_currentIndexChanged(int index)
{//"采样点格式" 下拉列表框
    switch(index)    //采样点格式， 更新" 每采样点字节数" SpinBox的显示值
    {
    case 0:     //Unknow
    case 1:     //UInt8
        ui->spinBytesPerSamp->setValue(1); break;
    case 2:     //Int16
        ui->spinBytesPerSamp->setValue(2); break;
    case 3:     //Int32
    case 4:     //Float
        ui->spinBytesPerSamp->setValue(4);
    }

    int bytes=ui->spinChanCount->value() * ui->spinBytesPerSamp->value();
    ui->spinBytesPerFrame->setValue(bytes);   //更新每帧字节数

    bool canShowWave= (index==1) && (ui->spinChanCount->value() ==1);  //是否可以显示曲线
    ui->chkBoxShowWave->setEnabled(canShowWave);   //只有UInt8, 通道数为1 时才能显示曲线
    if (!canShowWave)
        ui->chkBoxShowWave->setChecked(false);    //不能显示曲线
}

void MainWindow::on_spinChanCount_valueChanged(int arg1)
{//"通道数"  SpinBox
    ui->spinBytesPerFrame->setValue( arg1 * (ui->spinBytesPerSamp->value()));  //计算每帧字节数

    bool canShowWave= (arg1==1) && (ui->comboSampFormat->currentIndex() ==1);
    ui->chkBoxShowWave->setEnabled(canShowWave);   //只有UInt8, 通道数为1 时才能显示曲线
    if (!canShowWave)
        ui->chkBoxShowWave->setChecked(false);     //不能显示曲线
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_isWorking)
    {
        QMessageBox::information(this,"提示","正在采集或播放音频，不允许退出");
        event->ignore();
    }
    else
        event->accept();
}

void MainWindow::on_actPlayFile_triggered()
{//"播放文件"按钮
    QString filename =ui->editFileName->text().trimmed();
    if (filename.isEmpty() ||   !QFile::exists(filename))  //检查文件
    {
        QMessageBox::critical(this,"错误","文件名为空，或文件不存在");
        return;
    }

    sinkFileDevice.setFileName(filename);   //文件IO设备设置文件
    if ( !sinkFileDevice.open(QIODeviceBase::ReadOnly))   //以只读方式打开
    {
        QMessageBox::critical(this,"错误","打开文件时出现错误，无法播放");
        return;
    }

    QAudioFormat format;    //使用界面上的音频格式参数
    format.setSampleRate(ui->spinSampRate->value());
    format.setChannelCount(ui->spinChanCount->value());
    int index=ui->comboSampFormat->currentIndex();
    format.setSampleFormat(QAudioFormat::SampleFormat(index));

    QAudioDevice audioDevice= QMediaDevices::defaultAudioOutput();  //默认的音频输出设备
    if (!audioDevice.isFormatSupported(format))   //是否支持此音频格式参数
    {
        QMessageBox::critical(this,"错误","播放设备不支持此音频格式设置，无法播放");
        return;
    }

    audioSink =new QAudioSink(format, this);   //创建audioSink
    connect(audioSink, &QAudioSink::stateChanged,this, &MainWindow::do_sink_stateChanged);
    audioSink->start(&sinkFileDevice);    //开始播放


    m_isWorking=true;       //表示有设备在工作，不能关闭窗口
    ui->actPlayFile->setEnabled(false);
}


void MainWindow::on_actPreferFormat_triggered()
{//"首先音频格式"按钮, 显示默认音频输入设备的首选音频格式
    QAudioFormat audioFormat =QMediaDevices::defaultAudioInput().preferredFormat();
    ui->spinSampRate->setValue(audioFormat.sampleRate());   //采样频率
    ui->comboSampFormat->setCurrentIndex(audioFormat.sampleFormat());   //采样点格式
    ui->spinChanCount->setValue(audioFormat.channelCount());            //通道个数
    ui->spinBytesPerSamp->setValue(audioFormat.bytesPerSample());       //每个采样点的字节数
    ui->spinBytesPerFrame->setValue(audioFormat.bytesPerFrame());       //每帧字节数
}



