#include "mainwindow.h"
#include "ui_mainwindow.h"
#include    <QDir>
#include    <QMessageBox>
#include    <QDesktopServices>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->editURL->setClearButtonEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_finished()
{//网络响应结束
    QFileInfo fileInfo(downloadedFile->fileName());   //为了获取下载后的文件名

    downloadedFile->close();
    delete downloadedFile;   //删除临时文件对象
    reply->deleteLater(); //由主事件循环删除此对象

    if (ui->chkBoxOpen->isChecked())    //打开下载的文件
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    ui->btnDownload->setEnabled(true);
}

void MainWindow::do_readyRead()
{//读取下载的数据
    downloadedFile->write(reply->readAll());
}

void MainWindow::do_downloadProgress(qint64 bytesRead, qint64 totalBytes)
{//下载进度
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void MainWindow::on_btnDefaultPath_clicked()
{//"默认路径"  按钮
    QString  curPath=QDir::currentPath();
    QDir    dir(curPath);
    QString  sub="temp";
    dir.mkdir(sub);
    ui->editPath->setText(curPath+"/"+sub+"/");
}

void MainWindow::on_btnDownload_clicked()
{//"下载"按钮，开始下载
    QString urlSpec = ui->editURL->text().trimmed();
    if (urlSpec.isEmpty())
    {
        QMessageBox::information(this, "错误","请指定需要下载的URL");
        return;
    }
    QUrl newUrl = QUrl::fromUserInput(urlSpec);  //URL地址
    if (!newUrl.isValid())
    {
        QString info="无效URL:"+urlSpec+"\n错误信息:"+newUrl.errorString();
        QMessageBox::information(this, "错误",info);
        return;
    }
    QString tempDir =ui->editPath->text().trimmed();    //临时目录
    if (tempDir.isEmpty())
    {
        QMessageBox::information(this, "错误", "请指定保存下载文件的目录");
        return;
    }

    QString fullFileName =tempDir+newUrl.fileName(); //文件名
    if (QFile::exists(fullFileName))
        QFile::remove(fullFileName);

    downloadedFile =new QFile(fullFileName);        //创建临时文件
    if (!downloadedFile->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, "错误","临时文件打开错误");
        return;
    }

    ui->btnDownload->setEnabled(false);
    //发送网络请求，创建网络响应
    reply = networkManager.get(QNetworkRequest(newUrl));
    connect(reply, SIGNAL(readyRead()), this, SLOT(do_readyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(do_downloadProgress(qint64,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(do_finished()));
}

void MainWindow::on_editURL_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
}


