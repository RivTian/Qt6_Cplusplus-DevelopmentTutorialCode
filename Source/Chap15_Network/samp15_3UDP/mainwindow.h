#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QUdpSocket>
#include    <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel  *labSocketState;    //状态栏上的标签
    QUdpSocket  *udpSocket;
    QString getLocalIP();       //获取本机IP地址

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //自定义槽函数
    void    do_socketStateChange(QAbstractSocket::SocketState socketState);

    void    do_socketReadyRead();//读取socket传入的数据
    //
    void on_actStart_triggered();

    void on_actStop_triggered();

    void on_actClear_triggered();

    void on_btnSend_clicked();

    void on_btnBroadcast_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
