#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDir>
#include    <QFileDialog>
#include    <QMessageBox>

#include    <QFontDialog>
#include    <QColorDialog>

#include    <QFloat16>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnFile_clicked()
{//"测试用文件"按钮
    QString curPath=QDir::currentPath();
	m_filename=QFileDialog::getSaveFileName(this,"选择一个文件",curPath,
                                            "流数据文件(*.stream)");

    if (!m_filename.isEmpty())
    {
        ui->editFilename->setText(m_filename);
        ui->groupBox->setEnabled(true);   //工作区可用
        ui->actSaveALL->setEnabled(true);
        ui->actReadALL->setEnabled(true);
    }
}

void MainWindow::on_btnInt8_Write_clicked()
{//qint8 类型数据 写入
    qint8 Value=ui->spin_Int8->value();
    writeByStream(Value);
}

void MainWindow::on_btnInt8_Read_clicked()
{//qint8类型数据， 读出
    qint8 Value=0;
    readByStream(Value);
    ui->edit_Int8->setText(QString("%1").arg(Value));
}

void MainWindow::on_btnUInt8_Write_clicked()
{//quint8类型数据
    quint8 Value=ui->spin_UInt8->value();
    writeByStream(Value);
}

void MainWindow::on_btnUInt8_Read_clicked()
{
    quint8 Value=0;
    readByStream(Value);
    ui->edit_UInt8->setText(QString("%1").arg(Value));
}

void MainWindow::on_btnInt16_Write_clicked()
{
    qint16 Value=ui->spin_Int16->value();
    writeByStream(Value);
}

void MainWindow::on_btnInt16_Read_clicked()
{
    qint16 Value=0;
    readByStream(Value);
    ui->edit_Int16->setText(QString("%1").arg(Value));
}

void MainWindow::on_btnUInt16_Write_clicked()
{//quint16类型数据 写入
    quint16 Value=ui->spin_UInt16->value();
    writeByStream(Value);
}

void MainWindow::on_btnUIn16_Read_clicked()
{
    quint16 Value;
    readByStream(Value);
    ui->edit_UInt16->setText(QString("%1").arg(Value));
}

template<class T>
void MainWindow::writeByStream(T Value)
{
    QFile fileDevice(m_filename);    //定义QFile对象，设置文件名
    if (!fileDevice.open(QIODevice::WriteOnly))  //以WriteOnly方式打开
        return;

    QDataStream fileStream(&fileDevice);         //创建QDataStream对象
    fileStream.setVersion(QDataStream::Qt_6_2);  //设置版本号，写入和读取的版本要兼容
	if (ui->radio_BigEndian->isChecked())		 //设置字节序
		fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if (ui->radio_Single->isChecked())           //设置float和double的精度
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    //数据写入部分
    fileStream<<Value;       //用流写入操作符写入
    fileDevice.close();
    //   QMessageBox::information(this,"消息","数据写入完成.");
}

template<class T>
void MainWindow::readByStream(T &Value)
{
    if (!QFile::exists(m_filename))
    {
        QMessageBox::critical(this,"错误","文件不存在,文件名：\n"+m_filename);
        return;
    }

    QFile fileDevice(m_filename);
    if (!fileDevice.open(QIODevice::ReadOnly))
        return;

    QDataStream fileStream(&fileDevice);
    fileStream.setVersion(QDataStream::Qt_6_2);     //设置版本号
    if (ui->radio_BigEndian->isChecked())
        fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if (ui->radio_Single->isChecked())
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    fileStream>>Value;      //读出数据
    fileDevice.close();
    //   QMessageBox::information(this,"消息","从文件读取数据完成.");
}

void MainWindow::on_btnInt32_Write_clicked()
{//qint32类型数据
    qint32 Value=ui->spin_Int32->value();
    writeByStream(Value);
}

void MainWindow::on_btnInt32_Read_clicked()
{
    qint32 Value=0;
    readByStream(Value);
    ui->edit_Int32->setText(QString("%1").arg(Value));
}

void MainWindow::on_btnInt64_Write_clicked()
{
    qint64 Value=ui->spin_Int64->value();
    writeByStream(Value);
}

void MainWindow::on_btnInt64_Read_clicked()
{//qint64类型数据，读出
    qint64 Value=0;
    readByStream(Value);
    ui->edit_Int64->setText(QString("%1").arg(Value));
}

void MainWindow::on_btnInt_Write_clicked()
{//qfloat16类型数据 写入
    qfloat16  Value=ui->spin_Float16->value();
    writeByStream(Value);
}

void MainWindow::on_btnInt_Read_clicked()
{//qfloat16类型数据，读出
    qfloat16 Value=0;
    readByStream(Value);
    float val=Value;    //转为float类型，因为QString的函数不支持qfloat16
    ui->edit_Float16->setText(QString::asprintf("%.2f",val));
}

void MainWindow::on_btnBool_Write_clicked()
{//bool类型数据 写入
    bool Value=ui->chkBox_In->isChecked();
    writeByStream(Value);
}

void MainWindow::on_btnBool_Read_clicked()
{//bool类型数据，读出
    bool Value=false;
    readByStream(Value);
    ui->chkBox_Out->setChecked(Value);
}

void MainWindow::on_btnFloat_Write_clicked()
{//float类型数据 写入
    float Value=ui->spin_Float->value();
    writeByStream(Value);
}

void MainWindow::on_btnFloat_Read_clicked()
{//float类型数据，读出
    float Value=0;
    readByStream(Value);
    ui->edit_Float->setText(QString::asprintf("%.4f",Value));
}

void MainWindow::on_btnDouble_Write_clicked()
{//double类型数据 写入
    double Value=ui->spin_Double->value();
    writeByStream(Value);
}

void MainWindow::on_btnDouble_Read_clicked()
{//double类型数据，读出
    double Value=0;
    readByStream(Value);
    ui->edit_Double->setText(QString::asprintf("%.4f",Value));
}

void MainWindow::on_btnQStr_Write_clicked()
{//QString 字符串 写入
    QString Value=ui->editQStr_In->text();
    writeByStream(Value);
}

void MainWindow::on_btnQStr_Read_clicked()
{//QString类型字符串，读出
    QString Value="";
    readByStream(Value);
    ui->editQStr_Out->setText(Value);
}

void MainWindow::on_btnFont_In_clicked()
{//"选择字体"按钮
    QFont font=ui->btnFont_In->font();
    bool  OK=false;
    font=QFontDialog::getFont(&OK,font,this);
    if(OK)
        ui->btnFont_In->setFont(font);
}

void MainWindow::on_btnFont_Write_clicked()
{// QFont类型数据 写入
    QFont font=ui->btnFont_In->font();
    writeByStream(font);
}

void MainWindow::on_btnFont_Read_clicked()
{//QFont类型数据，读出
    QFont font;
    readByStream(font);
    ui->editFont_Out->setFont(font);    //设置为编辑框的字体
}

void MainWindow::on_btnColor_In_clicked()
{//"选择颜色"按钮
    QPalette   plet=ui->btnColor_In->palette();
    QColor color=plet.buttonText().color();
    color= QColorDialog::getColor(color);
    if (color.isValid())
    {
        plet.setColor(QPalette::ButtonText,color);
        ui->btnColor_In->setPalette(plet);
    }
}

void MainWindow::on_btnColor_Write_clicked()
{//QColor类型数据 写入
    QPalette   plet=ui->btnColor_In->palette();
    QColor color=plet.buttonText().color();
    writeByStream(color);
}

void MainWindow::on_btnColor_Read_clicked()
{//QColor类型数据，读出
    QColor color=Qt::black;
    readByStream(color);

    QPalette   plet=ui->editColor_Out->palette();
    plet.setColor(QPalette::Text,color);
    ui->editColor_Out->setPalette(plet);    //设置为编辑框的文字颜色
}

void MainWindow::on_actClearOutput_triggered()
{ //“编辑框全清空”按钮
    ui->edit_Int8->clear();
    ui->edit_UInt8->clear();
    ui->edit_Int16->clear();
    ui->edit_UInt16->clear();
    ui->edit_Int32->clear();
    ui->edit_Int64->clear();

    ui->edit_Float16->clear();
    ui->edit_Float->clear();
    ui->edit_Double->clear();

    ui->editStr_Out->clear();
    ui->editQStr_Out->clear();

    QFont font=this->font();
    ui->editFont_Out->setFont(font);

    QPalette   plet=this->palette();
    ui->editColor_Out->setPalette(plet);
}

void MainWindow::on_actSaveALL_triggered()
{//"连续写入文件"按钮
    QFile fileDevice(m_filename);
    if (!fileDevice.open(QIODevice::WriteOnly))
        return;

    QDataStream fileStream(&fileDevice);
	fileStream.setVersion(QDataStream::Qt_6_2); //设置版本号
    if (ui->radio_BigEndian->isChecked())       //设置字节序
        fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if (ui->radio_Single->isChecked())           //设置浮点数精度
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    //数据写入部分
    qint8 int8_Value=ui->spin_Int8->value();
    fileStream<<int8_Value;

    quint8 uint8_Value=ui->spin_UInt8->value();
    fileStream<<uint8_Value;

    qint16 int16_Value=ui->spin_Int16->value();
    fileStream<<int16_Value;

    quint16 uint16_Value=ui->spin_UInt16->value();
    fileStream<<uint16_Value;

    qint32 int32_Value=ui->spin_Int32->value();
    fileStream<<int32_Value;

    qint64 int64_Value=ui->spin_Int64->value();
    fileStream<<int64_Value;

    bool bool_Value=ui->chkBox_In->isChecked();
    fileStream<<bool_Value;

    qfloat16 float16_Value=ui->spin_Float16->value();
    fileStream<<float16_Value;

    float float_Value=ui->spin_Float->value();
    fileStream<<float_Value;

    double double_Value=ui->spin_Double->value();
    fileStream<<double_Value;

    QString str=ui->editStr_In->text();
    char* charStr=str.toLocal8Bit().data();
    fileStream<<charStr;

    QString str_Value=ui->editQStr_In->text();
    fileStream<<str_Value;

    QFont font=ui->btnFont_In->font();
    fileStream<<font;

    QPalette   plet=ui->btnColor_In->palette();
    QColor color=plet.buttonText().color();
    fileStream<<color;

    fileDevice.close();
    QMessageBox::information(this,"消息","数据连续写入文件完成.");
}

void MainWindow::on_actReadALL_triggered()
{//"连续从文件读取"按钮
    if (!QFile::exists(m_filename))
    {
        QMessageBox::critical(this,"错误","文件不存在,文件名：\n"+m_filename);
        return;
    }

    QFile fileDevice(m_filename);
    if (!fileDevice.open(QIODevice::ReadOnly))  //以只读方式打开文件
        return;

    QDataStream fileStream(&fileDevice);
	fileStream.setVersion(QDataStream::Qt_6_2); //设置版本号
    if (ui->radio_BigEndian->isChecked())       //设置字节序
        fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if (ui->radio_Single->isChecked())          //设置浮点数精度
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    fileStream.startTransaction();  //开始读取操作事务
    //从文件按顺序读出各种数据
    qint8 int8_Value=0;
    fileStream>>int8_Value;      //qint8
    ui->edit_Int8->setText(QString("%1").arg(int8_Value));


    quint8 uint8_Value=0;
    fileStream>>uint8_Value;     //quint8
    ui->edit_UInt8->setText(QString("%1").arg(uint8_Value));

    qint16 int16_Value=0;
    fileStream>>int16_Value;     //qint16
    ui->edit_Int16->setText(QString("%1").arg(int16_Value));

    quint16 uint16_Value;
    fileStream>>uint16_Value;    //quint16
    ui->edit_UInt16->setText(QString("%1").arg(uint16_Value));

    qint32 int32_Value=0;
    fileStream>>int32_Value;     //qint32
    ui->edit_Int32->setText(QString("%1").arg(int32_Value));

    qint64 int64_Value=0;
    fileStream>>int64_Value;     //qint64
    ui->edit_Int64->setText(QString("%1").arg(int64_Value));

    bool bool_Value;
    fileStream>>bool_Value;      //bool
    ui->chkBox_Out->setChecked(bool_Value);

    qfloat16 float16_Value=0;
    fileStream>>float16_Value;   //qfloat16
    float val=float16_Value;
    ui->edit_Float16->setText(QString::asprintf("%.2f",val));

    float float_Value=0;
    fileStream>>float_Value;     //float
    ui->edit_Float->setText(QString::asprintf("%.4f",float_Value));

    double double_Value=0;
    fileStream>>double_Value;    //double
    ui->edit_Double->setText(QString::asprintf("%.4f",double_Value));

    char* charStr;
    fileStream>>charStr;         //char* 字符串
    QString str(charStr);        //转换为QString字符串
    ui->editStr_Out->setText(str);

    QString str_Value="";
    fileStream>>str_Value;       //QString
    ui->editQStr_Out->setText(str_Value);

    QFont font;
    fileStream>>font;            //QFont
    ui->editFont_Out->setFont(font);

    QColor color;
    fileStream>>color;           //QColor
    QPalette   plet=ui->editColor_Out->palette();
    plet.setColor(QPalette::Text,color);
    ui->editColor_Out->setPalette(plet);

    if (fileStream.commitTransaction()) //提交读取操作事务
        QMessageBox::information(this,"消息","从文件连续读取数据完成.");
    else
        QMessageBox::critical(this,"错误","文件读取过程出错，请检查文件格式");
    fileDevice.close();
}

void MainWindow::on_btnStr_Write_clicked()
{//char类型字符串  写入
    QString str=ui->editStr_In->text();
    char* Value=str.toLocal8Bit().data();
    writeByStream(Value);
}

void MainWindow::on_btnStr_Read_clicked()
{//char* 字符串，读出
    char* Value;
    readByStream(Value);
    QString str(Value);     //转为QString之后才能在界面上显示
    ui->editStr_Out->setText(str);
}

