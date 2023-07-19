#include "mainwindow.h"
#include "ui_mainwindow.h"

#include	<QSerialPortInfo>
#include	<QMessageBox>
#include    <QSettings>
#include	<QElapsedTimer>
#include	"tmylabel.h"


//void MainWindow::getCOMList()
//{
//	foreach(QSerialPortInfo portInfo, QSerialPortInfo::availablePorts())
//		ui->comboCOM_Num->addItem(portInfo.portName()+":"+portInfo.description());
//}

//UART发送 char* 字符串
void MainWindow::uartSend(QString cmd)
{
    ui->textCOM->appendPlainText(cmd);
//    const char *stdCmd=cmd.toLocal8Bit().data();        //转换为char*类型字符串
//    comPort.write(stdCmd);

    QByteArray bts=cmd.toLocal8Bit();   //转换为8位字符数据数组
    comPort.write(bts);
}

//发送编辑器里的AT指令
//void MainWindow::sendAT_Cmd(QLineEdit *edit)
//{
//    QString cmd=edit->text().trimmed();
//    if (ui->chkBox_NewLine2->isChecked())
//        cmd = cmd+"\r\n";

//    uartSend(cmd);
//}

void MainWindow::delayMs(int ms)
{
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed()<ms)
        QApplication::processEvents();
}

void MainWindow::loadFromReg()
{
//    QSettings  setting(QApplication::organizationName(), QApplication::applicationName());
    QSettings  setting;
    bool saved=setting.value("saved",false).toBool();
    if (!saved)
        return;

    //查找frame_CmdA中的QLineEdit对象，载入注册表里的数据
    QList<QLineEdit*> editList = ui->frame_CmdA->findChildren<QLineEdit*>();
    foreach(QLineEdit *edit, editList)
    {
        QString editName= edit->objectName();
        edit->setText(setting.value(editName).toString());
    }

    //查找frame_CmdB中的QLineEdit对象，载入注册表里的数据
    editList = ui->frame_CmdB->findChildren<QLineEdit*>();
    foreach(QLineEdit *edit, editList)
    {
        QString editName= edit->objectName();
        edit->setText(setting.value(editName).toString());
    }

    ui->comboWiFi_UartBuad->setCurrentText(setting.value("Uart_Rate").toString());

    ui->editAP_Name->setText(setting.value("AP_Name").toString());
    ui->editAP_PWD->setText(setting.value("AP_PWD").toString());

    ui->editServer_IP->setText(setting.value("TCP_IP").toString());
    ui->spinServer_Port->setValue(setting.value("TCP_Port").toInt());
}

void MainWindow::saveToReg()
{
//    QSettings  setting(QApplication::organizationName(), QApplication::applicationName());
    QSettings  setting;
    setting.setValue("saved",true);

    QList<QLineEdit*> editList = ui->frame_CmdA->findChildren<QLineEdit*>();
    foreach(QLineEdit *edit, editList)
    {
        QString editName= edit->objectName();
        setting.setValue(editName,edit->text());
    }

    editList = ui->frame_CmdB->findChildren<QLineEdit*>();
    foreach(QLineEdit *edit, editList)
    {
        QString editName= edit->objectName();
        setting.setValue(editName,edit->text());
    }

    setting.setValue("Uart_Rate",ui->comboWiFi_UartBuad->currentText());
    setting.setValue("AP_Name",ui->editAP_Name->text());
    setting.setValue("AP_PWD",ui->editAP_PWD->text());
    setting.setValue("TCP_IP",ui->editServer_IP->text());
    setting.setValue("TCP_Port",ui->spinServer_Port->value());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (comPort.isOpen())	//确保串口关闭
        comPort.close();

    saveToReg();
    event->accept();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toolBox->setEnabled(false);		//先禁止操作
    ui->tabWidget->setEnabled(false);

    //显示串口列表
    foreach(QSerialPortInfo portInfo, QSerialPortInfo::availablePorts())
        ui->comboCom_Port->addItem(portInfo.portName()+":"+portInfo.description());
    ui->actCom_Open->setEnabled(ui->comboCom_Port->count()>0);	//
    connect(&comPort,&QIODevice::readyRead,this, &MainWindow::do_com_readyRead);

    //添加标准波特率
    ui->comboCom_Buad->clear();
    foreach(qint32 baud, QSerialPortInfo::standardBaudRates())
        ui->comboCom_Buad->addItem(QString::number(baud));
    ui->comboCom_Buad->setCurrentText("115200");		//默认使用115200

    //为frame_CmdA 和 frame_CmdB里面的 TMyLabel 的clicked()信号关联槽函数
    QList<TMyLabel*> labList = ui->frame_CmdA->findChildren<TMyLabel*>();
    foreach(TMyLabel *lab, labList)
        connect(lab, SIGNAL(clicked()), this, SLOT(do_label_clicked()));
    labList = ui->frame_CmdB->findChildren<TMyLabel*>();
    foreach(TMyLabel *lab, labList)
        connect(lab, SIGNAL(clicked()), this, SLOT(do_label_clicked()));

    QApplication::setOrganizationName("WWB-Qt");	//设置应用程序参数，用于注册表
    QApplication::setApplicationName("ESP8266");
    loadFromReg();		//从注册表载入数据
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_com_readyRead()
{
    QByteArray all=comPort.readAll();
    QString str(all);
    ui->textCOM->appendPlainText(str);
}

void MainWindow::do_label_clicked()
{
    TMyLabel *lab = static_cast<TMyLabel*>(sender());   //获取信号发射者
    QLineEdit *edit =static_cast<QLineEdit*>(lab->buddy());     //获取伙伴组件
    QString cmd=edit->text().trimmed();     //指令字符串
    if (ui->chkBox_NewLine2->isChecked())
        cmd = cmd+"\r\n";           //添加回车换行符

    uartSend(cmd);      //通过串口发送字符串数据
}


//查询WiFi模块的UART参数
void MainWindow::on_btnWF_UART_Qry_clicked()
{
    QString cmd="AT+UART?\r\n";
    uartSend(cmd);
}

//使WiFi模块重启,模块重启后可能会进入透传模式
void MainWindow::on_btnWF_Reset_clicked()
{
    QString cmd="AT+RST\r\n";
    uartSend(cmd);
}

//WiFi模块测试指令，返回OK就是测试成功
void MainWindow::on_btnWF_Test_clicked()
{
    QString cmd="AT\r\n";
    uartSend(cmd);
}

//设置UART参数
void MainWindow::on_btnWF_UART_Set_clicked()
{
    QString rate=ui->comboWiFi_UartBuad->currentText().trimmed();
    QString cmd="AT+UART="+rate+",8,1,0,0\r\n";
    uartSend(cmd);
}


//退出透传模式
void MainWindow::on_btnTrans_Exit_clicked()
{
    QString cmd="+++";
    uartSend(cmd);

    delayMs(2000);
    //  QApplication::processEvents();
    //	comPort.waitForReadyRead(3000);	//等待3000ms,直到有数据接收到
    //	QApplication::processEvents();

    cmd="AT+CIPMODE=0\r\n";
    uartSend(cmd);
}

//进入透传模式
void MainWindow::on_btnTrans_Enter_clicked()
{
    QString cmd="AT+CIPMODE=1\r\n";
    uartSend(cmd);

//        QApplication::processEvents();
//        comPort.waitForReadyRead(2000);	//等待2000ms,直到有数据接收到
//        QApplication::processEvents();    //这种处理方式错误

    delayMs(2000);
    cmd="AT+CIPSEND\r\n";
    uartSend(cmd);
}

//查询模块的MAC和IP地址
void MainWindow::on_btnTCP_GetMac_clicked()
{
    QString cmd="AT+CIFSR\r\n";
    uartSend(cmd);
}

//查询路由器参数
void MainWindow::on_btnWF_GetAP_clicked()
{
    QString cmd="AT+CWJAP?\r\n";
    uartSend(cmd);
}

//查询模块当前连接状态，
void MainWindow::on_btnTCP_Query_clicked()
{
    QString cmd="AT+CIPSTATUS\r\n";
    uartSend(cmd);
}

//将WiFi模块设置为STA
void MainWindow::on_btnWF_SetAsSTA_clicked()
{
    QString cmd="AT+CWMODE=1\r\n";
    uartSend(cmd);
}

// 设置路由器参数
// AT+CWJAP="LINK_WHY","pai06why"
void MainWindow::on_btnWF_SetAP_clicked()
{
    QString APname=ui->editAP_Name->text().trimmed();
    QString APpswd=ui->editAP_PWD->text().trimmed();
    QString cmd="AT+CWJAP=\""+ APname+"\","+"\""+APpswd+"\"\r\n";
    uartSend(cmd);
}

//设置TCP服务器参数
//	AT+CIPSTART="TCP","192.168.1.47",8080
void MainWindow::on_btnTCP_SetServer_clicked()
{
    QString ServerIP=ui->editServer_IP->text().trimmed();
    int serverPort=ui->spinServer_Port->value();
    QString strPort;
    strPort.setNum(serverPort);

    QString cmd="AT+CIPSTART=\"TCP\",\""+ ServerIP+"\","+strPort+"\r\n";
    uartSend(cmd);
}

//恢复出厂设置,模块会重启
void MainWindow::on_pushButton_4_clicked()
{
    if (QMessageBox::question(this,"请确认","确定要恢复出厂设置吗？\r\n这将删除所有自定义设置",
                              QMessageBox::Yes|QMessageBox::Cancel) ==QMessageBox::Yes)
    {
        QString cmd="AT+RESTORE\r\n";
        uartSend(cmd);
    }
}

//设置为开机自动连接路由器和TCP服务器，并进入透传模式
//	AT+SAVETRANSLINK=1,"192.168.1.47",8080,"TCP"
void MainWindow::on_btnTrans_AutoOnReset_clicked()
{
    QString ServerIP=ui->editServer_IP->text().trimmed();
    int serverPort=ui->spinServer_Port->value();
    QString strPort;
    strPort.setNum(serverPort);

    QString cmd="AT+SAVETRANSLINK=1,\""+ ServerIP+"\","+strPort+",\"TCP\""+"\r\n";
    uartSend(cmd);
}

void MainWindow::on_btnNormSend_clicked()
{
    QString cmd=ui->plainText_Norm->toPlainText();
    if (ui->chkBox_NewLine1->isChecked())
        cmd = cmd+"\r\n";
    uartSend(cmd);
}


void MainWindow::on_btnTCP_AutoConn_clicked()
{
    QString cmd="AT+CWAUTOCONN=1\r\n";
    uartSend(cmd);
}

void MainWindow::on_actCom_Open_triggered()
{//"打开串口"按钮
    if (comPort.isOpen())
    {
        QMessageBox::warning(this,"错误","打开串口时出现错误");
        return;
    }

    QList<QSerialPortInfo>  comList=QSerialPortInfo::availablePorts();
    QSerialPortInfo portInfo=comList.at(ui->comboCom_Port->currentIndex());
    comPort.setPort(portInfo);      //设置使用哪个串口
//    comPort.setPortName(portInfo.portName());   //也可以设置串口名称

    //设置串口通信参数
    QString str=ui->comboCom_Buad->currentText();
    comPort.setBaudRate(str.toInt());		//设置波特率

    int value=ui->comboCom_DataBit->currentText().toInt();
    comPort.setDataBits(QSerialPort::DataBits(value));		//数据位，默认8位

    value=1+ui->comboCom_StopBit->currentIndex();
    comPort.setStopBits(QSerialPort::StopBits(value));		//停止位，默认1位

    if (ui->comboCom_Parity->currentIndex()==0)
        value=0;
    else
        value=1+ui->comboCom_Parity->currentIndex();
    comPort.setParity(QSerialPort::Parity(value));          //校验位，默认无

    if (comPort.open(QIODeviceBase::ReadWrite))
    {
        ui->frame_Port->setEnabled(false);  //串口设置面板
        ui->actCom_Open->setEnabled(false);
        ui->actCom_Close->setEnabled(true);

        ui->toolBox->setEnabled(true);      //ESP8266设置操作面板
        ui->tabWidget->setEnabled(true);
        QMessageBox::information(this,"提示信息","串口已经被成功打开");
    }
}


void MainWindow::on_actCom_Close_triggered()
{//"关闭串口"按钮
    if (comPort.isOpen())
    {
        comPort.close();

        ui->frame_Port->setEnabled(true);
        ui->actCom_Open->setEnabled(true);
        ui->actCom_Close->setEnabled(false);

        ui->toolBox->setEnabled(false);
        ui->tabWidget->setEnabled(false);
    }
}

void MainWindow::on_actClear_triggered()
{
    ui->textCOM->clear();
}

