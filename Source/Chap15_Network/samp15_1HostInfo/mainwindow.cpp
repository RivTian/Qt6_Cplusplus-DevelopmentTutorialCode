#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QHostInfo>
#include    <QNetworkInterface>


QString MainWindow::protocolName(QAbstractSocket::NetworkLayerProtocol protocol)
{//通过协议类型返回协议名称
    switch(protocol)
    {
    case QAbstractSocket::IPv4Protocol:
        return "IPv4";
    case QAbstractSocket::IPv6Protocol:
        return "IPv6";
    case QAbstractSocket::AnyIPProtocol:
        return "Any Internet Protocol";
    default:
        return "Unknown Network Layer Protocol";
    }
}

QString MainWindow::interfaceType(QNetworkInterface::InterfaceType type)
{//根据枚举值返回字符串
    switch(type)
    {
    case QNetworkInterface::Unknown:
        return "Unknown";
    case QNetworkInterface::Loopback:
        return "Loopback";
    case QNetworkInterface::Ethernet:
        return "Ethernet";
    case QNetworkInterface::Wifi:
        return "Wifi";
    default:
        return "Other type";
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_lookedUpHostInfo(const QHostInfo &host)
{//查找主机信息的槽函数
    QList<QHostAddress> addrList=host.addresses();   //获取主机的地址列表
    if (addrList.isEmpty())
        return;
    foreach(QHostAddress host, addrList)
    {
        bool show=ui->chkBox_OnlyIPv4->isChecked();   //只显示IPv4
        show= show? (host.protocol()==QAbstractSocket::IPv4Protocol):true;
        if (show)
        {
            ui->textEdit->appendPlainText("协 议："+protocolName(host.protocol()));
            ui->textEdit->appendPlainText(host.toString());
            ui->textEdit->appendPlainText(QString("isGlobal()=%1\n").arg(host.isGlobal()));
        }
    }
}


void MainWindow::on_btnGetHostInfo_clicked()
{//"获取本机主机名和IP地址" 按钮
    ui->textEdit->clear();
    QString hostName=QHostInfo::localHostName();    //本地主机名
    ui->textEdit->appendPlainText("本机主机名："+hostName+"\n");

    QHostInfo   hostInfo=QHostInfo::fromName(hostName);  //本机IP地址
    QList<QHostAddress> addrList=hostInfo.addresses();    //IP地址列表
    if (addrList.isEmpty())
        return;

    foreach (QHostAddress host, addrList)
    {
        bool show=ui->chkBox_OnlyIPv4->isChecked();   //只显示IPv4
        show= show? (host.protocol()==QAbstractSocket::IPv4Protocol):true;
        if (show)
        {
            ui->textEdit->appendPlainText("协 议："+protocolName(host.protocol())); //协议类型
            ui->textEdit->appendPlainText("本机IP地址："+host.toString());           //IP地址
            ui->textEdit->appendPlainText(QString("isGlobal()=%1\n").arg(host.isGlobal()));
//            ui->textEdit->appendPlainText(QString("isBroadcast()=%1").arg(host.isBroadcast()));
//            ui->textEdit->appendPlainText(QString("isLoopback()=%1").arg(host.isLoopback()));
//            ui->textEdit->appendPlainText(QString("isMulticast()=%1\n").arg(host.isMulticast()));
        }
    }
}


void MainWindow::on_btnLookup_clicked()
{//"查找域名的IP地址" 按钮
    ui->textEdit->clear();
    QString hostname=ui->comboBox->currentText();     //读取主机名
    ui->textEdit->appendPlainText("正在查找查找主机信息："+hostname);
    QHostInfo::lookupHost(hostname,this,SLOT(do_lookedUpHostInfo(QHostInfo)));
}


void MainWindow::on_btnAllAddress_clicked()
{//"allAddresses()"按钮
    ui->textEdit->clear();
    QList<QHostAddress> addrList=QNetworkInterface::allAddresses();
    if (addrList.isEmpty())
        return;
    foreach (QHostAddress host, addrList)
    {
        bool show=ui->chkBox_OnlyIPv4->isChecked();//只显示IPv4
        show= show? (host.protocol()==QAbstractSocket::IPv4Protocol):true;
        if (show)
        {
            ui->textEdit->appendPlainText("协  议："+protocolName(host.protocol()));
            ui->textEdit->appendPlainText("IP地址："+host.toString());
            ui->textEdit->appendPlainText(QString("isGlobal()=%1\n").arg(host.isGlobal()));
        }
    }
}


void MainWindow::on_btnAllInterface_clicked()
{//"allInterfaces()"按钮
    ui->textEdit->clear();
    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();   //网络接口列表
    foreach(QNetworkInterface interface,list)
    {
        if (!interface.isValid())
           continue;

        ui->textEdit->appendPlainText("设备名称："+interface.humanReadableName());
        ui->textEdit->appendPlainText("硬件地址："+interface.hardwareAddress());
        ui->textEdit->appendPlainText("接口类型："+interfaceType(interface.type()));
        QList<QNetworkAddressEntry> entryList=interface.addressEntries();   //地址列表
        foreach(QNetworkAddressEntry entry, entryList)
        {
            ui->textEdit->appendPlainText("   IP 地址："+entry.ip().toString());
            ui->textEdit->appendPlainText("   子网掩码："+entry.netmask().toString());
            ui->textEdit->appendPlainText("   广播地址："+entry.broadcast().toString()+"\n");
        }
    }
}

