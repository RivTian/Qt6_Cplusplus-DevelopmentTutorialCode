#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDir>
#include    <QFileDialog>
#include    <QTextStream>
#include    <QTextDocument>
#include    <QTextBlock>

bool MainWindow::openByIO_Whole(const QString &aFileName)
{//用IODevice方式打开文本文件
    QFile   aFile(aFileName);
    if (!aFile.exists()) //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;


    QByteArray all_Lines = aFile.readAll();    //自动添加
    QString text(all_Lines);                    //从字节数组转换为字符串
    ui->textEditDevice->setPlainText(text);
//    ui->textEditDevice->setPlainText(aFile.readAll());        //一行语句替代

    aFile.close();

    ui->tabWidget->setCurrentIndex(0);
    return  true;
}

bool MainWindow::openByIO_Lines(const QString &aFileName)
{
    QFile   aFile;
    aFile.setFileName(aFileName);
    if (!aFile.exists()) //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

//逐行读取
    ui->textEditDevice->clear();
    while (!aFile.atEnd())
    {
        QByteArray line = aFile.readLine();    //读取一行ASCII码，自动添加'\0'，相当于添加了一个空行
        QString str=QString::fromUtf8(line);   //从字节数组转换为字符串,文件必须是UTF-8编码
        str.truncate(str.length()-1);          //去除结尾增加的空行
        ui->textEditDevice->appendPlainText(str);
    }
    aFile.close();

    ui->tabWidget->setCurrentIndex(0);
    return  true;
}

bool MainWindow::openTextByStream(const QString &aFileName)
{ //用 QTextStream打开文本文件
    QFile   aFile(aFileName);

    if (!aFile.exists()) //文件不存在
        return false;

    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream aStream(&aFile); //用文本流读取文件
//    aStream.setAutoDetectUnicode(true); //自动检测Unicode,才能正常显示文档内的汉字

    ui->textEditStream->setPlainText(aStream.readAll());

//    ui->textEditStream->clear();//清空
//    while (!aStream.atEnd())
//    {
//        str=aStream.readLine();//读取文件的一行
//        ui->textEditStream->appendPlainText(str); //添加到文本框显示
//    }

    aFile.close();//关闭文件
    ui->tabWidget->setCurrentIndex(1);
    return  true;
}

bool MainWindow::saveTextByIODevice(const QString &aFileName)
{ //用IODevice方式保存文本文件
    QFile   aFile(aFileName);
//    aFile.setFileName(aFileName);

    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QString str=ui->textEditDevice->toPlainText();//整个内容作为字符串

    QByteArray  strBytes=str.toUtf8();//转换为字节数组
//    QByteArray  strBytes=str.toLocal8Bit();

    aFile.write(strBytes,strBytes.length());  //写入文件

    aFile.close();
    ui->tabWidget->setCurrentIndex(0);
    return  true;
}

bool MainWindow::saveTextByStream(const QString &aFileName)
{//用QTextStream保存文本文件
    QFile   aFile(aFileName);

    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream aStream(&aFile); //用文本流读取文件
//    aStream.setAutoDetectUnicode(true); //自动检测Unicode,才能正常显示文档内的汉字

    QString str=ui->textEditStream->toPlainText(); //转换为字符串

    aStream<<str; //写入文本流


//    QTextDocument   *doc;       //文本对象
//    QTextBlock      textLine;   //文本中的一段
//    doc=ui->textEditStream->document(); //QPlainTextEdit 的内容保存在一个 QTextDocument 里
//    int cnt=doc->blockCount();//QTextDocument分块保存内容，文本文件就是硬回车符是一个block,

//    QString str;
//    for (int i=0; i<cnt; i++) //扫描所有 blobk
//    {
//         textLine=doc->findBlockByNumber(i);//用blobk编号获取block，就是获取一行
//         str=textLine.text(); //转换为文本,末尾无\n
//         aStream<<str<<"\n";
//    }

    aFile.close();//关闭文件

    return  true;

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon  icon1(":/images/images/29.bmp");
    ui->tabWidget->setTabIcon(0,icon1);     //设置tab页图标

    QIcon  icon2(":/images/images/133.bmp");
    ui->tabWidget->setTabIcon(1,icon2);

    ui->tabWidget->setTabsClosable(false);  //不允许关闭tab页
    ui->tabWidget->setDocumentMode(true);   //文档模式，去掉了边框
    this->setCentralWidget(ui->tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpen_IODevice_triggered()
{//打开文件

    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="打开一个文件"; //对话框标题
    QString filter="程序文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if (aFileName.isEmpty())
        return;

    openByIO_Whole(aFileName);     //整体读取
//    openTextByIODevice2(aFileName);   //逐行读取
}

void MainWindow::on_actOpen_TextStream_triggered()
{ //打开文件
    QString curPath=QDir::currentPath();//获取系统当前目录
//调用打开文件对话框打开一个文件
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,
                 "程序文件(*.h *cpp);;文本文件(*.txt);;所有文件(*.*)");

    if (aFileName.isEmpty())
        return; //如果未选择文件，退出
    openTextByStream(aFileName); //打开文件
}


void MainWindow::on_actSave_IODevice_triggered()
{
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="另存为一个文件"; //对话框标题
    QString filter="h文件(*.h);;c++文件(*.cpp);;文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

    if (aFileName.isEmpty())
        return;

    saveTextByIODevice(aFileName);
}

void MainWindow::on_actSave_TextStream_triggered()
{
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle="另存为一个文件"; //对话框标题
    QString filter="h文件(*.h);;c++文件(*.cpp);;文本文件(*.txt);;所有文件(*.*)"; //文件过滤器

    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

    if (aFileName.isEmpty())
        return;

    saveTextByStream(aFileName);
}

