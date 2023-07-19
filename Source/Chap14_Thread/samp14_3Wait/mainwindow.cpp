#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (threadA->isRunning())
    {
        threadA->terminate();   //强制结束线程
        threadA->wait();        //等待线程结束
    }

    if (threadValue->isRunning())
    {
        threadValue->terminate();   //强制结束线程
        threadValue->wait();        //等待线程结束
    }

    if (threadPic->isRunning())
    {
        threadPic->terminate();   //强制结束线程
        threadPic->wait();        //等待线程结束
    }

    event->accept();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    threadA= new TDiceThread(this);    //producer
    threadValue= new TValueThread(this);   //consumer 1
    threadPic= new TPictureThread(this);     //consumer 2

    connect(threadA,&TDiceThread::started, this, &MainWindow::do_threadA_started);
    connect(threadA,&TDiceThread::finished,this, &MainWindow::do_threadA_finished);

    connect(threadValue,&TValueThread::newValue,this, &MainWindow::do_newValue);
    connect(threadPic,&TPictureThread::newPicture,this, &MainWindow::do_newPicture);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_threadA_started()
{//与线程的started()信号关联
    ui->statusbar->showMessage("Thread状态：thread started");
    ui->actThread_Run->setEnabled(false);
    ui->actThread_Quit->setEnabled(true);
}

void MainWindow::do_threadA_finished()
{//与线程的finished()信号关联
    ui->statusbar->showMessage("Thread状态：thread finished");
    ui->actThread_Run->setEnabled(true);
    ui->actThread_Quit->setEnabled(false);
}

void MainWindow::do_newValue(int seq, int diceValue)
{
    QString  str=QString::asprintf("第 %d 次掷骰子，点数为：%d",seq,diceValue);
    ui->plainTextEdit->appendPlainText(str);
}

void MainWindow::do_newPicture(QString &picName)
{
    QPixmap pic(picName);
    ui->labPic->setPixmap(pic);
}

void MainWindow::on_actClear_triggered()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_actThread_Run_triggered()
{//"启动线程"按钮
    threadValue->start();
    if (! threadPic->isRunning())
        threadPic->start();
    if(! threadA->isRunning())
        threadA->start();
}

void MainWindow::on_actThread_Quit_triggered()
{//"结束线程"按钮
    threadA->terminate();
    threadA->wait();
}
