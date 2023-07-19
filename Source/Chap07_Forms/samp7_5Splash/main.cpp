#include "mainwindow.h"

#include <QApplication>

#include    "tdialogLogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TDialogLogin   *dlgLogin=new TDialogLogin;  //创建Splash对话框

    if (dlgLogin->exec()==QDialog::Accepted)
    {
        MainWindow w;       //显示主窗口
        w.show();
        return a.exec();    //应用程序正常运行
    }
    else
        return  0;
}
