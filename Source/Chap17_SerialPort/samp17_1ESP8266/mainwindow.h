#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QSerialPort>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    QSerialPort comPort;	//串口对象

    void	uartSend(QString cmd);  //PC的UART发送QString数据

    //    void    sendAT_Cmd(QLineEdit *edit);

    void    delayMs(int ms);    //毫秒级延时

    void    loadFromReg();      //从注册表载入界面数据
    void    saveToReg();        //保存到注册表

    void    closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void do_com_readyRead();		//串口有数据可读

    void do_label_clicked();		//与TMyLabel的clicked()信号关联



    void on_btnWF_UART_Qry_clicked();

    void on_btnWF_Reset_clicked();

    void on_btnWF_Test_clicked();

    void on_btnWF_UART_Set_clicked();

    void on_btnTrans_Exit_clicked();

    void on_btnTrans_Enter_clicked();

    void on_btnTCP_GetMac_clicked();

    void on_btnWF_GetAP_clicked();

    void on_btnTCP_Query_clicked();

    void on_btnWF_SetAsSTA_clicked();

    void on_btnWF_SetAP_clicked();

    void on_btnTCP_SetServer_clicked();

    void on_pushButton_4_clicked();

    void on_btnTrans_AutoOnReset_clicked();

    void on_btnNormSend_clicked();

    void on_btnTCP_AutoConn_clicked();

    void on_actCom_Open_triggered();

    void on_actCom_Close_triggered();

    void on_actClear_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
