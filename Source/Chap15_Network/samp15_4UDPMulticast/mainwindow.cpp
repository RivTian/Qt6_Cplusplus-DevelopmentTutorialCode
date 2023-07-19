#include "mainwindow.h"
#include "ui_mainwindow.h"
#include    <QtNetwork>

QString MainWindow::getLocalIP()
{
    QString hostName=QHostInfo::localHostName();//本地主机名
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

    labSocketState=new QLabel("Socket状态：");//
    labSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(labSocketState);

    QString localIP=getLocalIP();   //本地主机名
    this->setWindowTitle(this->windowTitle()+"----本机IP地址："+localIP);

    udpSocket=new QUdpSocket(this);
    //Multicast路由层次，1表示只在同一局域网内
    //组播TTL: 生存时间，每跨1个路由会减1，多播无法跨过大多数路由所以为1
    //默认值是1，表示数据包只能在本地的子网中传送。
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);
    //    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,ui->spinTTL->value());

    connect(udpSocket,&QUdpSocket::stateChanged,this,&MainWindow::do_socketStateChange);
    do_socketStateChange(udpSocket->state());   //立即刷新一次

    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(do_socketReadyRead()));
}

MainWindow::~MainWindow()
{
//    udpSocket->abort();
//    delete udpSocket; //没必要
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
{//读取数据报
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray   datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress    peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
        //        udpSocket->readDatagram(datagram.data(),datagram.size());
        QString str=datagram.data();

        QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"] ";
        ui->textEdit->appendPlainText(peer+str);
    }
}

void MainWindow::on_actStart_triggered()
{//"加入组播"按钮
    QString   IP=ui->comboIP->currentText();
    groupAddress=QHostAddress(IP);      //多播组地址
    quint16   groupPort=ui->spinPort->value();    //端口
    if (udpSocket->bind(QHostAddress::AnyIPv4, groupPort, QUdpSocket::ShareAddress))
    {
        udpSocket->joinMulticastGroup(groupAddress); //加入多播组
        ui->textEdit->appendPlainText("**加入组播成功");
        ui->textEdit->appendPlainText("**组播地址IP："+IP);
        ui->textEdit->appendPlainText("**绑定端口："+QString::number(groupPort));
        ui->actStart->setEnabled(false);
        ui->actStop->setEnabled(true);
        ui->comboIP->setEnabled(false);
        ui->spinPort->setEnabled(false);
        ui->btnMulticast->setEnabled(true);
    }
    else
        ui->textEdit->appendPlainText("**绑定端口失败");
}

void MainWindow::on_actStop_triggered()
{//"退出组播"按钮
    udpSocket->leaveMulticastGroup(groupAddress);   //退出组播
    udpSocket->abort();     //解除绑定
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    ui->comboIP->setEnabled(true);
    ui->spinPort->setEnabled(true);
    ui->btnMulticast->setEnabled(false);
    ui->textEdit->appendPlainText("**已退出组播,解除端口绑定");
}

void MainWindow::on_actClear_triggered()
{
    ui->textEdit->clear();
}

void MainWindow::on_actHostInfo_triggered()
{
    QString  hostName=QHostInfo::localHostName();//本地主机名
    ui->textEdit->appendPlainText("本机主机名："+hostName+"\n");
    QHostInfo  hostInfo=QHostInfo::fromName(hostName);
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


void MainWindow::on_btnMulticast_clicked()
{//"组播消息"按钮, 发送组播消息
    quint16  groupPort=ui->spinPort->value();  //组播端口
    QString  msg=ui->editMsg->text();
    QByteArray  datagram=msg.toUtf8();

    udpSocket->writeDatagram(datagram, groupAddress, groupPort);
    ui->textEdit->appendPlainText("[multicast] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}
