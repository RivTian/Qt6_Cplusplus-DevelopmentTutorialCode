#include    "mainwindow.h"

#include    <QApplication>

#include    <QTranslator>
#include    <QSettings>

QTranslator trans;     //应用程序的全局变量

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName("WWB-Qt");
    QApplication::setApplicationName("samp18_1");
//    QSettings  settings(QApplication::organizationName(),QApplication::applicationName());
    QSettings  settings;
    QString  curLang=settings.value("Language","CN").toString();    //读取注册表，EN或CN
    bool suss=false;
    if (curLang=="EN")
        suss =trans.load("samp18_1_en.qm");
    else
        suss =trans.load("samp18_1_cn.qm");

    if (suss)
        app.installTranslator(&trans);   //应用程序加载翻译器

    MainWindow w;
    w.setLanguage(curLang);
    w.show();
    return app.exec();
}
