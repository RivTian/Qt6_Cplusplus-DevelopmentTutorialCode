#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if(argc>1)
    {
        QString filename=argv[1];   //获取文件名
        w.openDataFile(filename);
    }

    w.show();
    return a.exec();
}
