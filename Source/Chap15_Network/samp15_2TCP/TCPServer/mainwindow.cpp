#include "mainwindow.h"
#include "ui_mainwindow.h"
#include    <QtNetwork>


QString MainWindow::getLocalIP()
{//获取本机IPv4地址
    QString hostName=QHostInfo::localHostName();    //本地主机名
    QHostInfo hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";

    QList<QHostAddress> addList=hostInfo.addresses();  //本机IP地址列表
    if (addList.isEmpty())
        return localIP;

    foreach(QHostAddress aHost, addList)
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    return localIP;
}

//void MainWindow::closeEvent(QCloseEvent *event)
//{//关闭窗口时停止监听
//    if (tcpServer->isListening())
//        tcpServer->close();;//停止网络监听
//    event->accept();
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labListen=new QLabel("监听状态:");
    labListen->setMinimumWidth(150);
    ui->statusBar->addWidget(labListen);

    labSocketState=new QLabel("Socket状态：");
    labSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(labSocketState);

    QString localIP=getLocalIP();   //获取本机IP
    this->setWindowTitle(this->windowTitle()+"----本机IP地址："+localIP);
    ui->comboIP->addItem(localIP);

    tcpServer=new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(do_newConnection()));
}

MainWindow::~MainWindow()
{//析构函数
    if (tcpSocket != nullptr)
    {
        if (tcpSocket->state()==QAbstractSocket::ConnectedState)
            tcpSocket->disconnectFromHost();    //断开与客户端的连接
    }

    if (tcpServer->isListening())
        tcpServer->close();     //停止网络监听

    delete ui;
}

void MainWindow::do_newConnection()
{
    tcpSocket = tcpServer->nextPendingConnection(); //创建socket

    connect(tcpSocket, SIGNAL(connected()),this, SLOT(do_clientConnected()));
    do_clientConnected();   //执行一次槽函数，显示状态

    connect(tcpSocket, SIGNAL(disconnected()),this, SLOT(do_clientDisconnected()));

    connect(tcpSocket,&QTcpSocket::stateChanged,this,&MainWindow::do_socketStateChange);
    do_socketStateChange(tcpSocket->state());   //执行一次槽函数，显示状态

    connect(tcpSocket,SIGNAL(readyRead()),  this,SLOT(do_socketReadyRead()));
}

void MainWindow::do_socketStateChange(QAbstractSocket::SocketState socketState)
{//socket状态变化时
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        labSocketState->setText("socket状态：UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        labSocketState->setText("socket状态：HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        labSocketState->setText("socket状态：ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        labSocketState->setText("socket状态：ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        labSocketState->setText("socket状态：BoundState");
        break;
    case QAbstractSocket::ClosingState:
        labSocketState->setText("socket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        labSocketState->setText("socket状态：ListeningState");
    }
}

void MainWindow::do_clientConnected()
{//客户端接入时
    ui->textEdit->appendPlainText("**client socket connected");
    ui->textEdit->appendPlainText("**peer address:"+
                                 tcpSocket->peerAddress().toString());
    ui->textEdit->appendPlainText("**peer port:"+
                                 QString::number(tcpSocket->peerPort()));
}

void MainWindow::do_clientDisconnected()
{//客户端断开连接时
    ui->textEdit->appendPlainText("**client socket disconnected");
    tcpSocket->deleteLater();
}

void MainWindow::do_socketReadyRead()
{//读取缓冲区的行文本
    while(tcpSocket->canReadLine())
        ui->textEdit->appendPlainText("[in] "+tcpSocket->readLine());
}

void MainWindow::on_actStart_triggered()
{//"开始监听"按钮
    QString  IP=ui->comboIP->currentText();  //IP地址字符串,如"127.0.0.1"
    quint16  port=ui->spinPort->value();     //端口
    QHostAddress   address(IP);
    tcpServer->listen(address,port);    //开始监听
    //    tcpServer->listen(QHostAddress::LocalHost,port);// Equivalent to QHostAddress("127.0.0.1").
    ui->textEdit->appendPlainText("**开始监听...");
    ui->textEdit->appendPlainText("**服务器地址："+tcpServer->serverAddress().toString());
    ui->textEdit->appendPlainText("**服务器端口："+QString::number(tcpServer->serverPort()));

    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);
    labListen->setText("监听状态：正在监听");
}

void MainWindow::on_actStop_triggered()
{//"停止监听"按钮
    if (tcpServer->isListening())   //tcpServer正在监听
    {
        if (tcpSocket != nullptr)
            if (tcpSocket->state()==QAbstractSocket::ConnectedState)
                tcpSocket->disconnectFromHost();
        tcpServer->close();         //停止监听
        ui->actStart->setEnabled(true);
        ui->actStop->setEnabled(false);
        labListen->setText("监听状态：已停止监听");
    }
}

void MainWindow::on_actClear_triggered()
{
    ui->textEdit->clear();
}

void MainWindow::on_btnSend_clicked()
{//"发送消息"按钮，发送一行字符串，以换行符结束
    QString  msg=ui->editMsg->text();
    ui->textEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();

    QByteArray  str=msg.toUtf8();
    str.append('\n');       //添加一个换行符
    tcpSocket->write(str);
}

void MainWindow::on_actHostInfo_triggered()
{//获取本机地址, 用于测试
    QString hostName=QHostInfo::localHostName();//本地主机名
    ui->textEdit->appendPlainText("本机主机名："+hostName+"\n");
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
    QList<QHostAddress> addList=hostInfo.addresses();
    if (addList.isEmpty())
        return;

    foreach (QHostAddress aHost, addList)
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            QString IP=aHost.toString();
            ui->textEdit->appendPlainText("本机IP地址："+aHost.toString());
            if (ui->comboIP->findText(IP)<0)
                ui->comboIP->addItem(IP);
        }
}

