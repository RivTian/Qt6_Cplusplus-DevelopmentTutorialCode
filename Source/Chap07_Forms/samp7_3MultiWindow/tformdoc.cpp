#include "tformdoc.h"
#include "ui_tformdoc.h"

#include    <QToolBar>
#include    <QVBoxLayout>
#include    <QFileDialog>
#include    <QFile>
#include    <QTextStream>
#include    <QFontDialog>
#include    <QFileInfo>
#include    <QMessageBox>

#include    <mainwindow.h>

TFormDoc::TFormDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TFormDoc)
{
    ui->setupUi(this);

    //使用UI设计的Actions设计工具栏
    QToolBar* locToolBar = new QToolBar("文档",this); //创建工具栏
    locToolBar->addAction(ui->actOpen);
    //    locToolBar->addAction(ui->actSave);
    locToolBar->addAction(ui->actFont);
    locToolBar->addSeparator();
    locToolBar->addAction(ui->actCut);
    locToolBar->addAction(ui->actCopy);
    locToolBar->addAction(ui->actPaste);
    locToolBar->addAction(ui->actUndo);
    locToolBar->addAction(ui->actRedo);
    locToolBar->addSeparator();
    locToolBar->addAction(ui->actClose);

    locToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    locToolBar->setAutoFillBackground(true);

    QVBoxLayout *Layout = new QVBoxLayout();
    Layout->addWidget(locToolBar);          //设置工具栏和plainTextEdit垂直布局
    Layout->addWidget(ui->plainTextEdit);
    Layout->setContentsMargins(2,2,2,2);    //减小边框的宽度
    Layout->setSpacing(2);
    this->setLayout(Layout); //设置布局
    //    this->setAutoFillBackground(true); //避免工具栏显示背景图片
}

TFormDoc::~TFormDoc()
{
    //    QMessageBox::information(this, "消息", "TFormDoc对象被删除和释放");
    delete ui;
}

void TFormDoc::loadFromFile(QString &aFileName)
{
//    QString str;
    QFile aFile(aFileName);  //以文件方式读出
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        QTextStream aStream(&aFile);    //用文本流读取文件
        ui->plainTextEdit->clear();     //清空
        while (!aStream.atEnd())
        {
            QString str=aStream.readLine();     //读取文件的一行
            ui->plainTextEdit->appendPlainText(str); //添加到文本框显示
        }
        aFile.close();      //关闭文件

        QFileInfo   fileInfo(aFileName);
        QString shortName=fileInfo.fileName();
        this->setWindowTitle(shortName);
        emit titleChanged(shortName);
    }
}

void TFormDoc::on_actOpen_triggered()
{
    //    curPath=QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString curPath=QDir::currentPath();
    //调用打开文件对话框打开一个文件
    QString aFileName=QFileDialog::getOpenFileName(this,tr("打开一个文件"),curPath,
                                                   "C程序文件(*.h *cpp);;文本文件(*.txt);;所有文件(*.*)");

    if (aFileName.isEmpty())
        return; //如果未选择文件，退出

    loadFromFile(aFileName);
}

void TFormDoc::on_actFont_triggered()
{
    QFont font=ui->plainTextEdit->font();

    bool    ok;
    font=QFontDialog::getFont(&ok,font);
    ui->plainTextEdit->setFont(font);

}

