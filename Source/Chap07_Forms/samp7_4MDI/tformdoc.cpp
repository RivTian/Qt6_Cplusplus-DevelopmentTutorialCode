#include "tformdoc.h"
#include "ui_tformdoc.h"

#include    <QFile>
#include    <QTextStream>
#include    <QFontDialog>
#include    <QFileInfo>
#include    <QMessageBox>

TFormDoc::TFormDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TFormDoc)
{
    ui->setupUi(this);

    this->setWindowTitle("New Doc[*]");    //窗口标题
    this->setAttribute(Qt::WA_DeleteOnClose);   //关闭时自动删除
    connect(ui->plainTextEdit, &QPlainTextEdit::modificationChanged,
            this, &QWidget::setWindowModified);
}

TFormDoc::~TFormDoc()
{
//    QMessageBox::information(this,"信息","文档窗口被释放");
    delete ui;
}

void TFormDoc::loadFromFile(QString &aFileName)
{//打开文件
    QFile aFile(aFileName);     //以文件方式读出
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        QTextStream aStream(&aFile);    //用文本流读取文件
        ui->plainTextEdit->clear();
        ui->plainTextEdit->setPlainText(aStream.readAll()); //读取文本文件
        aFile.close();

        m_filename=aFileName;             //保存当前文件名
        QFileInfo   fileInfo(aFileName);    //文件信息
        QString str=fileInfo.fileName();    //去除路径后的文件名
        this->setWindowTitle(str+"[*]");
        m_fileOpened=true;
    }
}

QString TFormDoc::currentFileName()
{
    return  m_filename;
}

bool TFormDoc::isFileOpened()
{ //文件是否已打开
    return m_fileOpened;
}

void TFormDoc::saveToFile()
{
//没有执行具体的保存操作
    this->setWindowModified(false);
}

void TFormDoc::setEditFont()
{
    QFont   font;
    font=ui->plainTextEdit->font();

    bool    ok;
    font=QFontDialog::getFont(&ok,font);
    ui->plainTextEdit->setFont(font);
}

void TFormDoc::textCut()
{
    ui->plainTextEdit->cut();
}

void TFormDoc::textCopy()
{
    ui->plainTextEdit->copy();
}

void TFormDoc::textPaste()
{
    ui->plainTextEdit->paste();
}
