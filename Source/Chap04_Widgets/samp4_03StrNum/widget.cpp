#include "widget.h"
#include "ui_widget.h"

//#include    <QString>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//"计算总价"按键
void Widget::on_btnCal_clicked()
{
    int num= ui->editNum->text().toInt();            //数量：字符串转换为整数
    float price= ui->editPrice->text().toFloat();    //单价：字符串转换为浮点数

    float total= num*price;
    QString str;
    str= str.setNum(total,'f',2);    //浮点数2位小数
    ui->editTotal->setText(str);
}

//读取十进制数，转换为其他进制
void Widget::on_btnDec_clicked()
{
    int val= ui->editDec->text().toInt();    //读取十进制数

    QString str= QString::number(val,16);    //显示为十六进制字符串
    str= str.toUpper();              //转换为大写字母
    ui->editHex->setText(str);

    str= QString::number(val,2);     //显示为二进制字符串
    ui->editBin->setText(str);
}

//读取二进制数，转换为其他进制的数
void Widget::on_btnBin_clicked()
{
    bool ok;
    int val= ui->editBin->text().toInt(&ok,2);   //以二进制数读入

    QString str= QString::number(val,10);        //显示为十进制字符串
    ui->editDec->setText(str);

    str= QString::number(val,16);    //显示为十六进制字符串
    str= str.toUpper();              //转换为大写字母
    ui->editHex->setText(str);
}

//读取十六进制数，转换为其他进制的数
void Widget::on_btnHex_clicked()
{
    bool ok;
    int val= ui->editHex->text().toInt(&ok,16);  //以十六进制数读入

    QString str= QString::number(val,10);        //显示为十进制字符串
    ui->editDec->setText(str);

    str= QString::number(val,2);     //显示为二进制字符串
    ui->editBin->setText(str);
}

void Widget::on_btnDebug_clicked()
{
//======字符串转换为整数
//    QString str;
//    bool ok=false;
//    int N;
////1. 默认十进制转换
//    str="153";
//    N=str.toInt(&ok);
//    qDebug("str= %s",str.toLocal8Bit().data());
//    qDebug("ok= %d, N= %d",ok,N);

////2. 使用十六进制转换
//    str="FF";
//    N=str.toInt(&ok,16);    //按16进制转换
//    qDebug("str= %s",str.toLocal8Bit().data());
//    qDebug("ok= %d, N= %d",ok,N);

////3. 使用二进制转换
//    str="10110111";
//    N=str.toInt(&ok,2);    //按2进制转换
//    qDebug("str= %s",str.toLocal8Bit().data());
//    qDebug("ok= %d, N= %d",ok,N);

////4. 按C语言规则
//    str="0x5F";
//    N=str.toInt(&ok,0);    //按C语言规则
//    qDebug("str= %s",str.toLocal8Bit().data());
//    qDebug("ok= %d, N= %d",ok,N);


//=====setNum()函数,整数
//    QString str;
//    int N=243;
//    str.setNum(N);
//    qDebug("N= %d",N);
//    qDebug("str= %s",str.toLocal8Bit().data());

//    str.setNum(N,16);
//    qDebug("str= %s",str.toLocal8Bit().data());

//    str.setNum(N,2);
//    qDebug("str= %s",str.toLocal8Bit().data());


//=====setNum()函数,浮点数
    QString str;
    double  num= 1245.2783;
    qDebug("num= %f",num);

    str.setNum(num,'f',5);    //小数点后5位, str= "1245.27830"
    qDebug("str= %s",str.toLocal8Bit().data());

    str.setNum(num,'E',5);    //小数点后是5位, str= "1.24528E+03"
    qDebug("str= %s",str.toLocal8Bit().data());

    str.setNum(num,'g',5);    //整数和小数位总共5位, str= "1245.3"
    qDebug("str= %s",str.toLocal8Bit().data());

    str.setNum(num,'g',3);    //整数和小数位总共3位, str= "1.25e+03"
    qDebug("str= %s",str.toLocal8Bit().data());


//=====静态函数number()函数
//    int N=245;
//    QString str=QString::number(203);   //转换为10进制字符串，str="245";
//    qDebug("N= %d",N);
//    qDebug("str= %s",str.toLocal8Bit().data());

//    str= QString::number(N,16);         //转换为16进制字符串，str="f5";
//    qDebug("str= %s",str.toLocal8Bit().data());

//    double  num=365.263;
//    qDebug("num= %f",num);

//    str= QString::number(num,'f',4);  //小数点后4位, str= "365.2630"
//    qDebug("str= %s",str.toLocal8Bit().data());

//    str= QString::number(num,'E',4);  //小数点后是4位, str= "3.6526E+02"
//    qDebug("str= %s",str.toLocal8Bit().data());

//====测试静态函数 asprintf()
//    QString str=QString::asprintf("Year=%d,\tMonth=%2d,\tDay=%2d",2021,6,12);
//    qDebug(str.toLocal8Bit().data());

//    QString UPC="UPC";
////    QString UPC="石油大学";     //不支持变量中有汉字
//    str=QString::asprintf("Hello,欢迎来到 %s",UPC.toLocal8Bit().data());    //Hello,欢迎来到 UPC
//    qDebug(str.toLocal8Bit().data());
//    ui->editHex->setText(str);

//    double pi=M_PI;     //圆周率常数pi，在<QtMath>中定义
//    str=QString::asprintf("Pi= %.10f",pi);
//    qDebug(str.toLocal8Bit().data());


//====测试函数 arg()
//    int Y=2021,M=8,D=3;
//    int base=10;
//    QChar ch('0');

//    QString str1=QString("%1年度: %3/%2/%1").arg(Y).arg(M,2,base,ch).arg(D,2,base,ch);
////    QString str1=QString("%1年%2月%3日").arg(Y).arg(M,2,base,ch).arg(D,2,base,ch);
//    qDebug(str1.toLocal8Bit().data());   //有汉字，会乱码
//    ui->editDec->setText(str1);

//    QString name="张三";
//    int age=25;
//    QString str2=QString("他名叫%1, 今年%2岁").arg(name).arg(age);
//    qDebug(str2.toLocal8Bit().data());   //有汉字，会乱码
//    ui->editBin->setText(str2);

//    double pi=M_PI;     //圆周率常数pi，在<QtMath>中定义
//    int precision=8;
//    QString str3=QString("pi=%1").arg(pi,0,'f',precision);
//    qDebug(str3.toLocal8Bit().data());   //有汉字，会乱码
}




