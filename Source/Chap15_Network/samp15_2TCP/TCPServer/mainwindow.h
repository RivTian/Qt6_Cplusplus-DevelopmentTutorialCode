#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QTcpServer>
#include    <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel  *labListen;         //状态栏标签
    QLabel  *labSocketState;    //状态栏标签

    QTcpServer *tcpServer;      //TCP服务器
    QTcpSocket *tcpSocket=nullptr;      //TCP通讯的Socket
    QString getLocalIP();       //获取本机IP地址

//protected:
//    void    closeEvent(QCloseEvent *event);   //close事件处理函数
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();      //析构函数里需要做一些处理，以保证断开与客户端的连接，并停止网络监听

private slots:
    //自定义槽函数
    void    do_newConnection();         //关联QTcpServer的newConnection()信号
    void    do_socketStateChange(QAbstractSocket::SocketState socketState);
    void    do_clientConnected();       //客户端 socket 已连接
    void    do_clientDisconnected();    //客户端 socket 已断开
    void    do_socketReadyRead();       //读取socket传入的数据

    //UI生成的
    void on_actStart_triggered();

    void on_actStop_triggered();

    void on_actClear_triggered();

    void on_btnSend_clicked();

    void on_actHostInfo_triggered();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
