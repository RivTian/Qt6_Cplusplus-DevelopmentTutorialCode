#include "mainwindow.h"
#include "ui_mainwindow.h"
#include    <QtNetwork>


QString MainWindow::getLocalIP()
{
    QString hostName=QHostInfo::localHostName();
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";
    QList<QHostAddress> addList=hostInfo.addresses();
    if (addList.isEmpty())
        return localIP;

    foreach (QHostAddress aHost,addList)
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    return localIP;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labSocketState=new QLabel("Socket状态：");
    labSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(labSocketState);

    QString localIP=getLocalIP();   //本机IP
    this->setWindowTitle(this->windowTitle()+"----本机IP地址："+localIP);
    ui->comboTargetIP->addItem(localIP);

    udpSocket=new QUdpSocket(this);   //创建socket

    connect(udpSocket,&QUdpSocket::stateChanged,this,&MainWindow::do_socketStateChange);
    do_socketStateChange(udpSocket->state());  //执行一次，显示当前状态

    connect(udpSocket,SIGNAL(readyRead()), this,SLOT(do_socketReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_socketStateChange(QAbstractSocket::SocketState socketState)
{
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

void MainWindow::do_socketReadyRead()
{//读取收到的数据报
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray   datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress    peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
        QString str=datagram.data();

        QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"] ";
        ui->textEdit->appendPlainText(peer+str);
    }
}

void MainWindow::on_actStart_triggered()
{//"绑定端口"按钮
    quint16 port=ui->spinBindPort->value();  //本机UDP端口
    if (udpSocket->bind(port))   //绑定端口成功
    {
        ui->textEdit->appendPlainText("**已成功绑定");
        ui->textEdit->appendPlainText("**绑定端口："
                        +QString::number(udpSocket->localPort()));
        ui->actStart->setEnabled(false);
        ui->actStop->setEnabled(true);
        ui->btnSend->setEnabled(true);
        ui->btnBroadcast->setEnabled(true);
    }
    else
        ui->textEdit->appendPlainText("**绑定失败");
}

void MainWindow::on_actStop_triggered()
{//"解除绑定"按钮
    udpSocket->abort();       //解除绑定，复位socket
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    ui->btnSend->setEnabled(false);
    ui->btnBroadcast->setEnabled(false);
    ui->textEdit->appendPlainText("**已解除绑定");
}

void MainWindow::on_actClear_triggered()
{
    ui->textEdit->clear();
}

void MainWindow::on_btnSend_clicked()
{//"发送消息" 按钮
    QString     targetIP=ui->comboTargetIP->currentText();  //目标IP
    QHostAddress  targetAddr(targetIP);
    quint16     targetPort=ui->spinTargetPort->value();     //目标port
    QString  msg=ui->editMsg->text();       //发送的消息内容

    QByteArray  str=msg.toUtf8();
    udpSocket->writeDatagram(str,targetAddr,targetPort); //发出数据报
    ui->textEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}

void MainWindow::on_btnBroadcast_clicked()
{ //"广播消息" 按钮
    quint16     targetPort=ui->spinTargetPort->value();   //目标端口
    QString  msg=ui->editMsg->text();
    QByteArray  str=msg.toUtf8();
    udpSocket->writeDatagram(str,QHostAddress::Broadcast,targetPort);

    ui->textEdit->appendPlainText("[broadcast] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}
