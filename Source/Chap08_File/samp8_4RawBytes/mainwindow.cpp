#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDir>
#include    <QFileDialog>
#include    <QMessageBox>
#include    <QFloat16>

bool MainWindow::iniWrite()
{//初始化写文件操作，创建QFile和QDataStream对象
    fileDevice=new QFile(m_filename);
    if (!fileDevice->open(QIODevice::WriteOnly))	//打开或创建文件
        return false;
    fileStream=new QDataStream(fileDevice);
    fileStream->setVersion(QDataStream::Qt_6_2);	//设置流版本号

    if (ui->radio_BigEndian->isChecked())			//设置字节序
        fileStream->setByteOrder(QDataStream::BigEndian);
    else
        fileStream->setByteOrder(QDataStream::LittleEndian);

    if (ui->radio_Single->isChecked())				//设置浮点数精度
        fileStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    return true;
}

bool MainWindow::iniRead()
{//初始化读文件操作，创建QFile和QDataStream对象
    if (!QFile::exists(m_filename))
    {
        QMessageBox::critical(this,"错误","文件不存在,文件名：\n"+m_filename);
        return false;
    }

    fileDevice=new QFile(m_filename);
    if (!fileDevice->open(QIODevice::ReadOnly))	 //打开文件
        return false;
    fileStream=new QDataStream(fileDevice);
    fileStream->setVersion(QDataStream::Qt_6_2); //设置流版本号
    if (ui->radio_BigEndian->isChecked())        //设置字节序
        fileStream->setByteOrder(QDataStream::BigEndian);
    else
        fileStream->setByteOrder(QDataStream::LittleEndian);

    if (ui->radio_Single->isChecked())			 //设置浮点数精度
        fileStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    return true;
}

void MainWindow::delFileStream()
{
    fileDevice->close();	//关闭文件
    delete fileStream;
    delete fileDevice;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox->setEnabled(false);    //工作区不允许操作
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnFile_clicked()
{//“测试用文件”按钮
    QString curPath=QDir::currentPath();
    //调用打开文件对话框打开一个文件
    m_filename=QFileDialog::getSaveFileName(this,"选择一个文件",curPath,"原始二进制数据文件(*.raw)");

    if (!m_filename.isEmpty())
    {
        ui->editFilename->setText(m_filename);
        ui->groupBox->setEnabled(true);
        ui->actSaveALL->setEnabled(true);
        ui->actReadALL->setEnabled(true);
    }
}

void MainWindow::on_btnInt8_Write_clicked()
{
    if (iniWrite())
    {
        qint8 Value=ui->spin_Int8->value();
        fileStream->writeRawData((char *)&Value,sizeof(qint8));
        delFileStream();
    }
}

void MainWindow::on_btnInt8_Read_clicked()
{
    if (iniRead())
    {
        qint8 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(qint8));
        ui->edit_Int8->setText(QString("%1").arg(Value));
        delFileStream();
    }
}

void MainWindow::on_btnUInt8_Write_clicked()
{//quint8
    if (iniWrite())
    {
        quint8 Value=ui->spin_UInt8->value();
        fileStream->writeRawData((char *)&Value,sizeof(quint8));
        delFileStream();
    }
}

void MainWindow::on_btnUInt8_Read_clicked()
{
    if (iniRead())
    {
        quint8 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(quint8));
        ui->edit_UInt8->setText(QString("%1").arg(Value));
        delFileStream();
    }
}

void MainWindow::on_btnInt16_Write_clicked()
{//qint16
    if (iniWrite())
    {
        qint16 Value=ui->spin_Int16->value();
        fileStream->writeRawData((char *)&Value,sizeof(qint16));
        delFileStream();
    }
}

void MainWindow::on_btnInt16_Read_clicked()
{
    if (iniRead())
    {
        qint16 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(qint16));
        ui->edit_Int16->setText(QString("%1").arg(Value));
        delFileStream();
    }
}

void MainWindow::on_btnUInt16_Write_clicked()
{//quint16 类型数据 写入
    if (iniWrite())
    {
        quint16 Value=ui->spin_UInt16->value();
        fileStream->writeRawData((char *)&Value,sizeof(quint16));
        delFileStream();
    }
}

void MainWindow::on_btnUIn16_Read_clicked()
{//quint16 类型数据 读出
    if (iniRead())
    {
        quint16 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(quint16));
        ui->edit_UInt16->setText(QString("%1").arg(Value));
        delFileStream();
    }
}

void MainWindow::on_btnInt32_Write_clicked()
{//qint32，写入
    if (iniWrite())     //初始化写文件操作，创建QFile和QDataStream对象
    {
        qint32 Value=ui->spin_Int32->value();
        fileStream->writeRawData((char *)&Value,sizeof(qint32));    //写qint32类型数据
        delFileStream();    //关闭文件，删除QFile和QDataStream对象
    }
}

void MainWindow::on_btnInt32_Read_clicked()
{//qint32 读出
    if (iniRead())      //初始化读文件操作，创建QFile和QDataStream对象
    {
        qint32 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(qint32));    //读qint32类型数据
        ui->edit_Int32->setText(QString("%1").arg(Value));
        delFileStream();    //关闭文件，删除QFile和QDataStream对象
    }
}

void MainWindow::on_btnInt64_Write_clicked()
{
    if (iniWrite())
    {
        qint64 Value=ui->spin_Int64->value();
        fileStream->writeRawData((char *)&Value,sizeof(qint64));
        delFileStream();
    }
}

void MainWindow::on_btnInt64_Read_clicked()
{
    if (iniRead())
    {
        qint64 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(qint64));
        ui->edit_Int64->setText(QString("%1").arg(Value));
        delFileStream();
    }
}

void MainWindow::on_btnInt_Write_clicked()
{
    if (iniWrite())
    {
        int Value=ui->spin_Int->value();
        fileStream->writeRawData((char *)&Value,sizeof(int));
        delFileStream();
    }
}

void MainWindow::on_btnInt_Read_clicked()
{
    if (iniRead())
    {
        int Value=0;
        fileStream->readRawData((char *)&Value, sizeof(int));
        ui->edit_Int->setText(QString("%1").arg(Value));
        delFileStream();
    }
}

void MainWindow::on_btnBool_Write_clicked()
{ //bool 写入
    if (iniWrite())
    {
        bool Value=ui->chkBox_In->isChecked();
        fileStream->writeRawData((char *)&Value,sizeof(bool));
        delFileStream();
    }
}

void MainWindow::on_btnBool_Read_clicked()
{// bool 读出
    if (iniRead())
    {
        bool Value=false;
        fileStream->readRawData((char *)&Value, sizeof(bool));
        ui->chkBox_Out->setChecked(Value);
        delFileStream();
    }
}

void MainWindow::on_btnFloat_Write_clicked()
{//float 写入
    if (iniWrite())
    {
        float Value=ui->spin_Float->value();
        fileStream->writeRawData((char *)&Value,sizeof(float));
        delFileStream();
    }
}

void MainWindow::on_btnFloat_Read_clicked()
{//float 读出
    if (iniRead())
    {
        float Value=0;
        fileStream->readRawData((char *)&Value, sizeof(float));
        ui->edit_Float->setText(QString::asprintf("%.4f",Value));
        delFileStream();
    }
}

void MainWindow::on_btnDouble_Write_clicked()
{//double 写入
    if (iniWrite())
    {
        double Value=ui->spin_Double->value();
        fileStream->writeRawData((char *)&Value,sizeof(double));
        delFileStream();
    }
}

void MainWindow::on_btnDouble_Read_clicked()
{//double 读出
    if (iniRead())
    {
        double Value=0;
        fileStream->readRawData((char *)&Value, sizeof(double));
        ui->edit_Double->setText(QString::asprintf("%.4f",Value));
        delFileStream();
    }
}

void MainWindow::on_btnStr_Write_clicked()
{//写入字符串，UTF8编码
    if (iniWrite())
    {
        QString str=ui->editStr_In->text();
        QByteArray   btArray=str.toUtf8();		//UTF8编码
        fileStream->writeBytes(btArray,btArray.length());
        delFileStream();
    }
}

void MainWindow::on_btnStr_Read_clicked()
{//读取字符串，UTF8编码
    if (iniRead())
    {
        char *buf;
        uint strLen;
        fileStream->readBytes(buf,strLen);   //同时读取字符串长度和字符串内容
        QString str=QString::fromUtf8(buf,strLen);	//用UTF8编码解读数据
        ui->editStr_Out->setText(str);
        delFileStream();
        delete buf;			//需要手动删除缓存区

        QString info=QString("读出数据缓冲区长度= %1 (字节)").arg(strLen);
        QMessageBox::information(this,"信息提示",info);
    }
}


void MainWindow::on_actClearOutput_triggered()
{
    ui->edit_Int8->clear();
    ui->edit_UInt8->clear();
    ui->edit_Int16->clear();
    ui->edit_UInt16->clear();
    ui->edit_Int32->clear();
    ui->edit_Int64->clear();
    ui->edit_Int->clear();

    ui->edit_Float->clear();
    ui->edit_Double->clear();

    ui->editStr_Out->clear();
}

void MainWindow::on_actSaveALL_triggered()
{//"连续写入文件"按钮
    if (!iniWrite())
        return;

    //数据写入
    qint8 int8_Value=ui->spin_Int8->value();
    fileStream->writeRawData((char *)&int8_Value,sizeof(qint8));

    quint8 uint8_Value=ui->spin_UInt8->value();
    fileStream->writeRawData((char *)&uint8_Value,sizeof(quint8));

    qint16 int16_Value=ui->spin_Int16->value();
    fileStream->writeRawData((char *)&int16_Value,sizeof(qint16));

    quint16 uint16_Value=ui->spin_UInt16->value();
    fileStream->writeRawData((char *)&uint16_Value,sizeof(quint16));

    qint32 int32_Value=ui->spin_Int32->value();
    fileStream->writeRawData((char *)&int32_Value,sizeof(qint32));

    qint64 int64_Value=ui->spin_Int64->value();
    fileStream->writeRawData((char *)&int64_Value,sizeof(qint64));

    int int_Value=ui->spin_Int->value();
    fileStream->writeRawData((char *)&int_Value,sizeof(int));

    bool bool_Value=ui->chkBox_In->isChecked();
    fileStream->writeRawData((char *)&bool_Value,sizeof(bool));

    qfloat16 float16_Value=ui->spin_Float16->value();
    fileStream->writeRawData((char *)&float16_Value,sizeof(qfloat16));

    float float_Value=ui->spin_Float->value();
    fileStream->writeRawData((char *)&float_Value,sizeof(float));

    double double_Value=ui->spin_Double->value();
    fileStream->writeRawData((char *)&double_Value,sizeof(double));

    QString str=ui->editStr_In->text();
    QByteArray   btArray=str.toUtf8();   //UTF8编码
    fileStream->writeBytes(btArray,btArray.length());

    str=ui->editStr_In2->text();
    btArray=str.toLatin1();              //Latin1编码
    fileStream->writeBytes(btArray,btArray.length());

    //数据写入完成
    delFileStream();
    QMessageBox::information(this,"消息","数据连续写入文件完成");
}

void MainWindow::on_actReadALL_triggered()
{// “连续从文件读取”按钮
    if (!iniRead())
        return;

    //数据读取
    qint8 int8_Value=0;
    fileStream->readRawData((char *)&int8_Value, sizeof(qint8));
    ui->edit_Int8->setText(QString("%1").arg(int8_Value));

    quint8 uint8_Value=0;
    fileStream->readRawData((char *)&uint8_Value, sizeof(quint8));
    ui->edit_UInt8->setText(QString("%1").arg(uint8_Value));

    qint16 int16_Value=0;
    fileStream->readRawData((char *)&int16_Value, sizeof(qint16));
    ui->edit_Int16->setText(QString("%1").arg(int16_Value));

    quint16 uint16_Value;
    fileStream->readRawData((char *)&uint16_Value, sizeof(quint16));
    ui->edit_UInt16->setText(QString("%1").arg(uint16_Value));

    qint32 int32_Value=0;
    fileStream->readRawData((char *)&int32_Value, sizeof(qint32));
    ui->edit_Int32->setText(QString("%1").arg(int32_Value));

    qint64 int64_Value=0;
    fileStream->readRawData((char *)&int64_Value, sizeof(qint64));
    ui->edit_Int64->setText(QString("%1").arg(int64_Value));

    int int_Value=0;
    fileStream->readRawData((char *)&int_Value, sizeof(int));
    ui->edit_Int->setText(QString("%1").arg(int_Value));

    bool bool_Value;
    fileStream->readRawData((char *)&bool_Value, sizeof(bool));
    ui->chkBox_Out->setChecked(bool_Value);

    qfloat16 float16_Value=0;
    fileStream->readRawData((char *)&float16_Value, sizeof(qfloat16));
    float val=float16_Value;    //asprintf()不支持qfloat16类型参数
    ui->edit_Float16->setText(QString::asprintf("%.2f",val));

    float float_Value=0;
    fileStream->readRawData((char *)&float_Value, sizeof(float));
    ui->edit_Float->setText(QString::asprintf("%.4f",float_Value));

    double double_Value=0;
    fileStream->readRawData((char *)&double_Value, sizeof(double));
    ui->edit_Double->setText(QString::asprintf("%.4f",double_Value));

    char *buf;
    uint strLen;
    fileStream->readBytes(buf,strLen);   //读取字符串
    QString str_Value=QString::fromUtf8(buf,strLen);	//用UTF8解码
    ui->editStr_Out->setText(str_Value);

    fileStream->readBytes(buf,strLen);   //读下一个字符串
    str_Value=QString::fromLatin1(buf,strLen);			//用Latin1解码
    ui->editStr_Out2->setText(str_Value);

    //读取完成
    delFileStream();
    QMessageBox::information(this,"消息","从文件连续读取数据完成.");
}


void MainWindow::on_btnStr_Read2_clicked()
{//读取字符串，Latin1编码
    if (iniRead())
    {
        char *buf;
        uint strLen;
        fileStream->readBytes(buf,strLen);   //同时读取字符串长度和字符串内容
        QString str=QString::fromLatin1(buf,strLen);	//用Latin1编码解读数据
        ui->editStr_Out2->setText(str);
        delFileStream();
        delete buf;      //需要手动删除缓存区

        QString info=QString("读出数据缓冲区长度= %1 (字节)").arg(strLen);
        QMessageBox::information(this,"信息提示",info);
    }
}

void MainWindow::on_btnStr_Write2_clicked()
{//写入字符串， Latin1编码
    if (iniWrite())
    {
        QString str=ui->editStr_In2->text();
        QByteArray   btArray=str.toLatin1();	//Latin1编码
        fileStream->writeBytes(btArray,btArray.length());
        delFileStream();
    }
}


void MainWindow::on_btnFloat16_Write_clicked()
{// qfloat16 写入
    if (iniWrite())
    {
        qfloat16 Value=ui->spin_Float16->value();
        fileStream->writeRawData((char *)&Value,sizeof(qfloat16));
        delFileStream();
    }
}


void MainWindow::on_btnFloat16_Read_clicked()
{//qfloat16 读出
    if (iniRead())
    {
        qfloat16 Value=0;
        fileStream->readRawData((char *)&Value, sizeof(qfloat16));
        float val=Value;
        ui->edit_Float16->setText(QString::asprintf("%.2f",val));//asprintf()不支持qfloat16类型参数
        delFileStream();
    }
}

