#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDir>
#include    <QFileDialog>
#include    <QTextStream>
#include    <QTextDocument>
#include    <QTextBlock>
#include    <QSaveFile>
#include    <QException>

bool MainWindow::openByIO_Whole(const QString &aFileName)
{//用IODevice方式打开文本文件，整体读取
    QFile   aFile(aFileName);
    if (!aFile.exists())    //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray all_Lines = aFile.readAll(); //读取全部内容
    QString text(all_Lines);                //将字节数组转换为字符串
    ui->textEditDevice->setPlainText(text);
    aFile.close();

    ui->tabWidget->setCurrentIndex(0);
    return  true;
}

bool MainWindow::openByIO_Lines(const QString &aFileName)
{//逐行读取
    QFile   aFile;
    aFile.setFileName(aFileName);
    if (!aFile.exists())    //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

//逐行读取
    ui->textEditDevice->clear();
    while (!aFile.atEnd())
    {
        QByteArray line = aFile.readLine();    //读取一行ASCII码，自动添加'\0'
        QString str=QString::fromUtf8(line);   //从字节数组转换为字符串,文件必须是UTF-8编码
        str.truncate(str.length()-1);          //去除增加的空行
        ui->textEditDevice->appendPlainText(str);
    }
    aFile.close();

    ui->tabWidget->setCurrentIndex(0);
    return  true;
}

bool MainWindow::openByStream_Whole(const QString &aFileName)
{ //用 QTextStream打开文本文件
    QFile   aFile(aFileName);
    if (!aFile.exists())
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream aStream(&aFile);            //用文本流读取文件内容
    aStream.setAutoDetectUnicode(true);     //自动检测Unicode
    QString  str=aStream.readAll();         //读取全部内容
    ui->textEditStream->setPlainText(str);

    aFile.close();
    ui->tabWidget->setCurrentIndex(1);
    return  true;
}

bool MainWindow::openByStream_Lines(const QString &aFileName)
{
    QFile   aFile(aFileName);
    if (!aFile.exists())    //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream aStream(&aFile);            //用文本流读取文件内容
    aStream.setAutoDetectUnicode(true);     //自动检测Unicode

    ui->textEditStream->clear();
    while (!aStream.atEnd())
    {
        QString str=aStream.readLine();     //读取一行文字
        ui->textEditStream->appendPlainText(str);   //添加到文本框显示
    }
    aFile.close();
    ui->tabWidget->setCurrentIndex(1);
    return  true;
}

bool MainWindow::saveByStream_Whole(const QString &aFileName)
{
    QFile   aFile(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream aStream(&aFile);            //用文本流读取文件内容
    aStream.setAutoDetectUnicode(true);     //自动检测Unicode
    QString str=ui->textEditStream->toPlainText();
    aStream<<str;   //写入文本流

    aFile.close();
    return  true;
}

bool MainWindow::saveByIO_Whole(const QString &aFileName)
{
    QFile   aFile(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QString str=ui->textEditDevice->toPlainText();  //整个内容作为字符串
    QByteArray  strBytes=str.toUtf8();              //转换为字节数组, UTF-8编码
    aFile.write(strBytes,strBytes.length());        //写入文件

    aFile.close();
    ui->tabWidget->setCurrentIndex(0);
    return  true;
}

//bool MainWindow::saveByIO_Document(const QString &aFileName)
//{
//    QFile   aFile(aFileName);
//    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
//        return false;
//////使用QTextDocument逐段读取总是不成功
//    QTextDocument *doc=ui->textEditStream->document();  //QPlainTextEdit 的内容保存在一个 QTextDocument 里
//    int cnt=doc->blockCount();      //QTextDocument分块保存内容，文本文件就是硬回车符是一个block,
//    qDebug("blockCount()= %d",cnt);
////    for (int i=0; i<cnt; i++)   //扫描所有blobk
////    {
////        QTextBlock textBlock= doc->findBlockByNumber(i);     //用blobk编号获取block
////        QString str=textBlock.text();               //转换为文本
////        QByteArray  strBytes=str.toUtf8();          //转换为字节数组, UTF-8编码
////        aFile.write(strBytes,strBytes.length());    //写入文件
////    }

////    QTextBlock textBlock=doc->firstBlock();
////    QTextBlock::iterator it;
////    for (it = textBlock.begin(); !it.atEnd(); ++it)
////    {
////        QString str=it.fragment().text();               //转换为文本
////        QByteArray  strBytes=str.toUtf8();          //转换为字节数组, UTF-8编码
////        aFile.write(strBytes,strBytes.length());    //写入文件
////    }

//    aFile.close();
//    ui->tabWidget->setCurrentIndex(0);
//    return  true;
//}

bool MainWindow::saveByIO_Safe(const QString &aFileName)
{//使用QSaveFile保存文件
    QSaveFile   aFile(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    aFile.setDirectWriteFallback(false);    //使用临时文件
    try
    {
        QString str=ui->textEditDevice->toPlainText();  //整个内容作为字符串
        QByteArray  strBytes=str.toUtf8();              //转换为字节数组, UTF-8编码
        aFile.write(strBytes,strBytes.length());        //写入文件
        aFile.commit();
        ui->tabWidget->setCurrentIndex(0);
        return  true;
    }
    catch (QException &e)
    {
        qDebug("保存文件的过程发生了错误");
        aFile.cancelWriting();      //出现异常时取消写入
        return false;
    }


//    try
//    {
//        QTextDocument *doc=ui->textEditStream->document();  //QPlainTextEdit 的内容保存在一个 QTextDocument 里
//        int cnt=doc->blockCount();      //QTextDocument分块保存内容，文本文件就是硬回车符是一个block,
//        for (int i=0; i<cnt; i++)   //扫描所有blobk
//        {
//            QTextBlock textBlock=doc->findBlockByNumber(i);     //用blobk编号获取block
//            QString str=textBlock.text();               //转换为文本
//            QByteArray  strBytes=str.toUtf8();          //转换为字节数组, UTF-8编码
//            aFile.write(strBytes, strBytes.length());    //写入文件
//        }

//        aFile.commit();     //提交修改
//        ui->tabWidget->setCurrentIndex(0);
//        return  true;
//    }
//    catch(QException &e)
//    {
//        qDebug("保存文件的过程发生了错误");
//        aFile.cancelWriting();      //出现一场时取消写入
//        return false;
//    }

}

bool MainWindow::saveByStream_Safe(const QString &aFileName)
{
    QSaveFile  aFile(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    try
    {   //逐段保存
        QTextStream aStream(&aFile);            //用文本流读取文件
        aStream.setAutoDetectUnicode(true);     //自动检测Unicode
        QTextDocument *doc=ui->textEditStream->document();     //QPlainTextEdit 的内容
        int cnt=doc->blockCount();              //硬回车符是一个block
        for (int i=0; i<cnt; i++)   //扫描所有 blobk
        {
            QTextBlock textLine=doc->findBlockByNumber(i);  //获取一段
            QString str=textLine.text();                    //提取文本,末尾无\n
            aStream<<str<<"\n";     //写入时增加一个换行符
        }
        aFile.commit();             //提交修改
        return  true;
    }
    catch (QException &e)
    {
        qDebug("保存文件的过程发生了错误");
        aFile.cancelWriting();      //出现异常时取消写入
        return false;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QIcon  icon1(":/images/images/29.bmp");
//    ui->tabWidget->setTabIcon(0,icon1);     //设置tab页图标

//    QIcon  icon2(":/images/images/133.bmp");
//    ui->tabWidget->setTabIcon(1,icon2);

    ui->tabWidget->setTabsClosable(false);  //不允许关闭tab页
    ui->tabWidget->setDocumentMode(true);   //文档模式，去掉了边框
    this->setCentralWidget(ui->tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpen_IODevice_triggered()
{//“QFile打开”按钮
    QString curPath=QDir::currentPath();    //获取应用程序当前目录
    QString dlgTitle="打开一个文件";
    QString filter="程序文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if (aFileName.isEmpty())
        return;

    QFileInfo  fileInfo(aFileName);
    QDir::setCurrent(fileInfo.absolutePath());//设置应用程序当前目录
    openByIO_Whole(aFileName);      //整体读取
//    openByIO_Lines(aFileName);   //逐行读取
}

void MainWindow::on_actOpen_TextStream_triggered()
{ //"QTextStream打开"按钮
    QString curPath=QDir::currentPath();    //获取应用程序当前目录
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,
                 "程序文件(*.h *cpp);;文本文件(*.txt);;所有文件(*.*)");

    if (aFileName.isEmpty())
        return;

    QFileInfo  fileInfo(aFileName);
    QDir::setCurrent(fileInfo.absolutePath());

//    openByStream_Whole(aFileName);      //打开文件,整体读取
    openByStream_Lines(aFileName);      //打开文件,逐行读取
}

void MainWindow::on_actSave_IODevice_triggered()
{//"QFile另存"按钮
    QString curPath=QDir::currentPath();    //获取应用程序当前目录
    QString dlgTitle="另存为一个文件";
    QString filter="h文件(*.h);;c++文件(*.cpp);;文本文件(*.txt);;所有文件(*.*)";
    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;

    QFileInfo  fileInfo(aFileName);
    QDir::setCurrent(fileInfo.absolutePath());//设置应用程序当前目录
    saveByIO_Whole(aFileName);    //整体保存
}

void MainWindow::on_actSave_TextStream_triggered()
{//"QTextStream另存"按钮
    QString curPath=QDir::currentPath();    //获取系统当前目录
    QString dlgTitle="另存为一个文件";
    QString filter="h文件(*.h);;c++文件(*.cpp);;文本文件(*.txt);;所有文件(*.*)";

    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

    if (aFileName.isEmpty())
        return;

    QFileInfo  fileInfo(aFileName);
    QDir::setCurrent(fileInfo.absolutePath());
//    saveByStream_Whole(aFileName);        //整体保存
    saveByStream_Safe(aFileName);           //逐段读取后保存
}


void MainWindow::on_actSave_TextSafe_triggered()
{//"QSaveFile另存"按钮
    QString curPath=QDir::currentPath();    //获取应用程序当前目录
    QString dlgTitle="另存为一个文件";
    QString filter="h文件(*.h);;c++文件(*.cpp);;文本文件(*.txt);;所有文件(*.*)";
    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;

    QFileInfo  fileInfo(aFileName);
    QDir::setCurrent(fileInfo.absolutePath());
    saveByIO_Safe(aFileName);   //使用QSaveFile类保存文件
}

