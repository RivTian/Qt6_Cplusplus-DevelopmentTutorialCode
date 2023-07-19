#ifndef TDIALOGLOGIN_H
#define TDIALOGLOGIN_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class TDialogLogin;}
QT_END_NAMESPACE


class TDialogLogin : public QDialog
{
    Q_OBJECT

private:
    bool    m_moving=false;     //表示窗口是否在鼠标操作下移动
    QPoint  m_lastPos;          //上一次的鼠标位置

    QString m_user="user";      //初始化用户名
    QString m_pswd="12345";     //初始化密码，未加密的

    int m_tryCount=0; //试错次数

    void    readSettings();     //读取设置,从注册表
    void    writeSettings();    //写入设置，从注册表
    QString encrypt(const QString& str);//字符串加密

protected:
//鼠标事件处理函数，用于拖动窗口
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);

public:
    TDialogLogin(QWidget *parent = nullptr);
    ~TDialogLogin();

private slots:
    void on_btnOK_clicked();    //"确定"按钮

private:
    Ui::TDialogLogin *ui;
};

#endif // TDIALOGLOGIN_H
